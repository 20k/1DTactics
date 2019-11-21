#include "combat_model.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <imgui/imgui.h>
#include "renderables.hpp"

std::string format_to_string(float val, int precision)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << val;
    return ss.str();
}

std::optional<item_facet> item::get_facet(item_facet::types type) const
{
    for(auto& i : facets)
    {
        if(i.type == type)
            return i;
    }

    return std::nullopt;
}

void item::add_facet(const item_facet& facet)
{
    if(get_facet(facet.type).has_value())
        throw std::runtime_error("Already has facet " + std::to_string(facet.type));

    facets.push_back(facet);
}

float body_part::get_functionality()
{
    float frac = hp / max_hp;

    if(frac >= 0.9)
        return 1;

    if(frac >= 0.75)
        return 0.8;

    if(frac >= 0.5)
        return 0.6;

    if(frac >= 0.25)
        return 0.3;

    if(frac > 0.0001f)
        return 0.05f;

    return 0.0f;
}

///crits?
combat_info::hit_types body_part::roll_to_hit(std::minstd_rand& rng, float probability_modification) const
{
    bool direct_hit = rand_det_s(rng, 0.f, 1.f) < (base_hit_probability * probability_modification);

    bool graze_hit = false;

    if(!direct_hit)
    {
        graze_hit = rand_det_s(rng, 0.f, 1.f) < (base_hit_probability * probability_modification);
    }

    if(direct_hit)
        return combat_info::DIRECT;

    if(graze_hit)
        return combat_info::GRAZE;

    return combat_info::MISS;
}

void body_part::hit(float damage)
{
    hp -= damage;

    hp = clamp(hp, 0, max_hp);
}

combat_info::hit_types creature_model::hit_random_bodypart_for(float damage, float probability_modification)
{
    ///uuh this is shit
    static std::minstd_rand rng(1234);

    std::vector<float> probabilities;

    for(auto& i : parts)
    {
        probabilities.push_back(i.base_hit_probability * probability_modification);
    }

    ///weights towards most hittable
    int random_part = random_select_with_weights(rng, probabilities);

    combat_info::hit_types ret = parts[random_part].roll_to_hit(rng, probability_modification);

    if(ret == combat_info::DIRECT)
    {
        parts[random_part].hit(damage);
    }

    ///handle graze. Probably separate defined damage

    return ret;
}

float creature_model::get_move_distance() const
{
    return 10.f;
}

void creature_model::add_item(const item& it)
{
    inventory.push_back(it);
}

void creature_model::append_rendering()
{
    ImGui::Text("Health:");

    for(int i=0; i < (int)parts.size(); i++)
    {
        const body_part& my_part = parts[i];

        std::string name = body_part_info::names[my_part.type];

        std::string hp_1 = format_to_string(my_part.hp, 2);
        std::string hp_2 = format_to_string(my_part.max_hp, 2);

        ImGui::Text((name + ": ").c_str());

        ImGui::SameLine();

        ImGui::Text(hp_1.c_str());

        ImGui::SameLine();

        ImGui::Text(hp_2.c_str());
    }
}

creature_model default_alien_model()
{
    creature_model mod;

    body_part leg;
    leg.type = body_part_info::LEG;
    leg.base_hit_probability = 0.6;

    body_part head;
    head.type = body_part_info::HEAD;
    head.base_hit_probability = 0.3;


    body_part torso;
    torso.max_hp = 4;
    torso.hp = 4;
    torso.type = body_part_info::TORSO;
    torso.base_hit_probability = 0.8;

    mod.parts.push_back(head);

    mod.parts.push_back(torso);

    mod.parts.push_back(leg);
    mod.parts.push_back(leg);
    mod.parts.push_back(leg);
    mod.parts.push_back(leg);

    mod.name = "Bug";

    return mod;
}

creature_model default_trooper_model()
{
    creature_model mod;

    body_part leg;
    leg.type = body_part_info::LEG;
    leg.base_hit_probability = 0.6;

    body_part arm;
    arm.type = body_part_info::ARM;
    arm.base_hit_probability = 0.5;

    body_part head;
    head.hp = 0.5;
    head.max_hp = 0.5;
    head.type = body_part_info::HEAD;
    head.base_hit_probability = 0.1;

    body_part torso;
    torso.max_hp = 2;
    torso.hp = 2;
    torso.type = body_part_info::TORSO;
    torso.base_hit_probability = 0.8;

    mod.parts.push_back(head);
    mod.parts.push_back(torso);

    mod.parts.push_back(arm);
    mod.parts.push_back(arm);

    mod.parts.push_back(leg);
    mod.parts.push_back(leg);

    mod.name = "Trooper";

    return mod;
}

item default_rifle()
{
    item_facet range;
    range.value = 20;
    range.type = item_facet::RANGE;

    item_facet damage;
    damage.value = 1;
    damage.type = item_facet::DAMAGE;

    item_facet agency;
    agency.value = 1;
    agency.type = item_facet::SHOOTABLE;

    item_facet accuracy_mod;
    accuracy_mod.value = 0.8;
    accuracy_mod.type = item_facet::ACCURACY_MOD;

    item ret;
    ret.add_facet(range);
    ret.add_facet(damage);
    ret.add_facet(agency);
    ret.add_facet(accuracy_mod);
    ret.name = "PoW_21";

    return ret;
}

///ignores item effects
float get_terrainwise_hit_probability(const entity_object& source, const entity_object& target, float weapon_shoot_distance)
{
    if(weapon_shoot_distance < 1)
        return 0;

    if(&source == &target)
        return 0;

    float hit_probability_at_max = 0.4;
    float hit_probability_at_min = 1;

    vec2f diff = (vec2f){source.tilemap_pos.x(), source.tilemap_pos.y()} - (vec2f){target.tilemap_pos.x(), target.tilemap_pos.y()};

    float len = diff.length();

    if(len <= weapon_shoot_distance)
    {
        ///so. at 0 len, we hit 1, and len = weapon_shoot_distance, we hit max = 0.1
        return mix(hit_probability_at_min, hit_probability_at_max, len / weapon_shoot_distance);
    }

    float extreme_range = 2;

    if(len <= weapon_shoot_distance + extreme_range)
    {
        return mix(hit_probability_at_max, 0.f, (len - weapon_shoot_distance) / extreme_range);
    }

    return 0.f;
}

void handle_attack(entity_object& source, entity_object& target, item& with)
{
    //std::cout << source.name << " shot " << target.name << std::endl;

    float accuracy_mod = 1;

    if(auto facet_val = with.get_facet(item_facet::ACCURACY_MOD); facet_val.has_value())
    {
        accuracy_mod = facet_val.value().value;
    }

    float damage = 0;

    if(auto facet_val = with.get_facet(item_facet::DAMAGE); facet_val.has_value())
    {
        damage = facet_val.value().value;
    }

    auto result = target.model.hit_random_bodypart_for(damage, accuracy_mod);

    //std::cout << "result " << result << std::endl;
}
