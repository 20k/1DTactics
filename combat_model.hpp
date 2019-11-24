#ifndef COMBAT_MODEL_HPP_INCLUDED
#define COMBAT_MODEL_HPP_INCLUDED

#include <vec/vec.hpp>
#include <optional>
#include <string>
#include <vector>

std::string format_to_string(float val, int precision);

namespace body_part_info
{
    enum types
    {
        ARM,
        LEG,
        HEAD,
        TORSO,
        COUNT
    };

    static inline
    std::vector<std::string> names
    {
        "Arm",
        "Leg",
        "Head",
        "Torso",
        "Error",
    };
}

namespace combat_info
{
    enum hit_types
    {
        DIRECT,
        GRAZE,
        MISS,
    };
}

struct item_facet
{
    enum types
    {
        RANGE,
        DAMAGE,
        ACCURACY_MOD,
        SHOOTABLE,
        WEARABLE,
        THROWABLE,
        MELEEABLE,
        NONE
    };

    float value = 0;
    types type = types::NONE;
};

struct item
{
    std::vector<item_facet> facets;
    std::string name;

    std::optional<item_facet> get_facet(item_facet::types type) const;
    void add_facet(const item_facet& facet);
};

struct body_part
{
    float hp = 1;
    float max_hp = 1;
    float base_hit_probability = 1; ///100%
    body_part_info::types type = body_part_info::COUNT;

    float get_functionality();
    combat_info::hit_types roll_to_hit(std::minstd_rand& gen, float probability_modification) const;
    void hit(float damage);
};

struct creature_ap
{
    int32_t current = 0;
    int32_t maximum = 4;
    int32_t current_regen = 3;
    int32_t base_regen = 3;

    enum types
    {
        MOVE,
        SHOOT,
        THROW,
        BRACE,
    };

    static inline
    std::vector<int32_t> costs
    {
        1,
        2,
        2,
        1,
    };

    bool can_do_type(types type);
    void do_type(types type);
    void do_regen();
};

struct playspace_manager;
struct dijkstras_info;

struct creature_model
{
    std::string name = "No Name";
    std::vector<body_part> parts;

    std::vector<item> inventory;
    creature_ap ap_model;

    combat_info::hit_types hit_random_bodypart_for(float damage, float probability_modification);
    float get_move_speed_fraction();
    float get_move_distance() const;

    void add_item(const item& it);
    ///renders current ui state into an existing window
    void append_rendering();

    dijkstras_info get_dijkstras_for_ap_move(playspace_manager& play, vec2i position, int ap_usage);

    /*float get_aim_accuracy_fraction();
    float get_shoot_range();
    float get_move_range();
    float get*/
};

struct entity_object;

float get_terrainwise_hit_probability(const entity_object& source, const entity_object& target, float weapon_shoot_distance);
float get_full_hit_probability(const entity_object& source, const entity_object& target, const item& it);
void handle_attack(entity_object& source, entity_object& target, item& with);

creature_model default_alien_model();
creature_model default_trooper_model();

item default_rifle();

#endif // COMBAT_MODEL_HPP_INCLUDED
