#include "renderables.hpp"
#include <assert.h>
#include <iostream>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>
#include "pathfinding.hpp"

template<typename T>
void add_to(T& in, vec2i loc)
{
    in.push_back(loc);
}

std::map<tiles::types, std::vector<vec2i>>& get_locations()
{
    static std::map<tiles::types, std::vector<vec2i>> ret;

    using namespace tiles;

    ///consider adding skull and crossbones to dirt
    add_to(ret[BASE], {0, 0});

    add_to(ret[DIRT], {1, 0});
    add_to(ret[DIRT], {2, 0});
    add_to(ret[DIRT], {3, 0});
    add_to(ret[DIRT], {4, 0});

    add_to(ret[GRASS], {5, 0});
    add_to(ret[GRASS], {6, 0});
    add_to(ret[GRASS], {7, 0});

    add_to(ret[TREE_1], {0, 1});
    add_to(ret[TREE_1], {1, 1});
    add_to(ret[TREE_2], {2, 1});
    add_to(ret[TREE_2], {4, 1});
    add_to(ret[TREE_DENSE], {3, 1});
    add_to(ret[TREE_DENSE], {3, 2});
    add_to(ret[TREE_ROUND], {5, 1});
    add_to(ret[TREE_ROUND], {4, 2});

    add_to(ret[CACTUS], {6, 1});
    add_to(ret[CACTUS], {7, 1});

    add_to(ret[VINE], {2, 2});
    add_to(ret[SHRUB], {0, 2});

    add_to(ret[BRAMBLE], {0, 2});
    add_to(ret[BRAMBLE], {0, 6});

    ///top left man is 24, 0

    add_to(ret[CIVILIAN], {25, 0});
    add_to(ret[CIVILIAN], {26, 0});
    add_to(ret[CIVILIAN], {26, 1});
    add_to(ret[CIVILIAN], {27, 1});
    add_to(ret[CIVILIAN], {29, 1});
    add_to(ret[CIVILIAN], {30, 1});
    add_to(ret[CIVILIAN], {30, 3});
    add_to(ret[CIVILIAN], {31, 3});
    add_to(ret[CIVILIAN], {30, 4});
    add_to(ret[CIVILIAN], {31, 4});
    ///top left scorpion is 25, 5

    add_to(ret[SOLDIER], {26, 4});
    add_to(ret[SOLDIER], {30, 9});

    add_to(ret[GROUND_BUG], {28, 5});
    add_to(ret[GROUND_BUG], {29, 5});
    add_to(ret[GROUND_BUG], {30, 5});
    add_to(ret[SMALL_PINCHY], {31, 5});
    add_to(ret[FLYING_BUG], {26, 5});
    add_to(ret[ARMOURED_BUG], {27, 5});
    add_to(ret[SCORPION], {24, 5});

    add_to(ret[LAND_ANIMAL], {25, 7});
    add_to(ret[LAND_ANIMAL], {26, 7});
    add_to(ret[LAND_ANIMAL], {27, 7});
    add_to(ret[LAND_ANIMAL], {28, 7});
    //add_to(ret[LAND_ANIMAL], {29, 7});
    add_to(ret[LAND_ANIMAL], {30, 7});
    add_to(ret[LAND_ANIMAL], {31, 7});
    add_to(ret[LAND_ANIMAL], {26, 8}); //bat

    add_to(ret[SEA_ANIMAL], {25, 8});
    add_to(ret[SEA_ANIMAL], {28, 8});
    add_to(ret[CROCODILE], {29, 8});

    add_to(ret[FACE_MALE], {24, 10});
    add_to(ret[FACE_MALE], {26, 10});
    add_to(ret[FACE_MALE], {27, 10});
    add_to(ret[FACE_MALE], {28, 10});
    add_to(ret[FACE_MALE], {29, 10});

    add_to(ret[FACE_WOMAN], {25, 10});
    add_to(ret[FACE_WOMAN], {30, 10});
    add_to(ret[FACE_WOMAN], {31, 10});

    add_to(ret[THIN_DOOR_CLOSED], {3, 4});
    add_to(ret[THIN_DOOR_OPEN], {4, 4});

    add_to(ret[DOOR_CLOSED], {3, 3});
    add_to(ret[THIN_DOOR_OPEN], {4, 3});

    add_to(ret[GRAVE], {0, 14});
    add_to(ret[GRAVE], {1, 14});
    add_to(ret[GRAVE], {2, 14});

    add_to(ret[WOOD_FENCE_FULL], {1, 3});
    add_to(ret[WOOD_FENCE_FULL], {2, 3});
    add_to(ret[WOOD_FENCE_HALF], {0, 3});

    add_to(ret[TILING_WALL], {16, 19});
    add_to(ret[TILING_WALL], {17, 19});
    add_to(ret[TILING_WALL], {18, 19});

    add_to(ret[TILING_WALL], {16, 20});
    add_to(ret[TILING_WALL], {17, 20});
    add_to(ret[TILING_WALL], {18, 20});

    add_to(ret[TILING_WALL], {16, 21});
    add_to(ret[TILING_WALL], {17, 21});
    add_to(ret[TILING_WALL], {18, 21});

    add_to(ret[CULTIVATION], {13, 6});
    add_to(ret[CULTIVATION], {14, 6});
    add_to(ret[CULTIVATION], {15, 6});
    add_to(ret[CULTIVATION], {16, 6});
    add_to(ret[CULTIVATION], {17, 6});

    return ret;
}

vec4f get_colour_of(tiles::types tile_type, level_info::types level_type)
{
    constexpr vec4f barren_col = srgb_to_lin_approx((vec4f){122, 68, 74, 255} / 255.f);
    constexpr vec4f grass_col = srgb_to_lin_approx((vec4f){56, 217, 115, 255} / 255.f);

    if(tile_type == tiles::BRAMBLE || tile_type == tiles::SHRUB || tile_type == tiles::BASE)
    {
        if(level_type == level_info::GRASS)
            return grass_col;
        else
            return barren_col;
    }

    if(tile_type == tiles::DIRT)
        return barren_col;

    if(tile_type == tiles::GRASS)
        return grass_col;

    if(tile_type == tiles::TREE_1 || tile_type == tiles::TREE_2 || tile_type == tiles::TREE_DENSE ||
       tile_type == tiles::TREE_ROUND || tile_type == tiles::CACTUS || tile_type == tiles::VINE ||
       tile_type == tiles::CULTIVATION || tile_type == tiles::CROCODILE)
       return grass_col;

    constexpr vec4f wood_col = srgb_to_lin_approx((vec4f){191, 121, 88, 255} / 255.f);
    constexpr vec4f building_gray = srgb_to_lin_approx((vec4f){207, 198, 184, 255} / 255.f);
    constexpr vec4f generic_red = srgb_to_lin_approx((vec4f){230, 72, 46, 255} / 255.f);

    if(tile_type == tiles::ROCKS || tile_type == tiles::GRAVE || tile_type == tiles::TILING_WALL)
        return building_gray;

    if(tile_type == tiles::LAND_ANIMAL)
        return building_gray;

    if(tile_type == tiles::SEA_ANIMAL)
        return building_gray;

    if(tile_type == tiles::CIVILIAN || tile_type == tiles::SOLDIER ||
       tile_type == tiles::GROUND_BUG || tile_type == tiles::FLYING_BUG || tile_type == tiles::ARMOURED_BUG || tile_type == tiles::SMALL_PINCHY)
        return building_gray;

    if(tile_type == tiles::SCORPION)
        return generic_red;

    if(tile_type == tiles::FACE_MALE || tile_type == tiles::FACE_WOMAN)
        return building_gray;

    if(tile_type == tiles::WOOD_FENCE_FULL || tile_type == tiles::WOOD_FENCE_HALF || tile_type == tiles::TILING_WALL ||
       tile_type == tiles::THIN_DOOR_CLOSED || tile_type == tiles::THIN_DOOR_OPEN ||
       tile_type == tiles::DOOR_CLOSED || tile_type == tiles::DOOR_OPEN)
        return wood_col;

    throw std::runtime_error("Did not find " + std::to_string(tile_type));
}

vec2i get_tile_of(tiles::types type)
{
    auto& tiles = get_locations();

    auto& which = tiles[type];

    if(which.size() == 0)
        throw std::runtime_error("No tiles for type " + std::to_string(type));

    int len = which.size();

    static std::minstd_rand rng;

    int iwhich = (int)rand_det_s(rng, 0, len);

    if(iwhich >= len || iwhich < 0)
        throw std::runtime_error("Rng is bad");

    return which[iwhich];
}

vec4f special_colour_randomisation(vec4f rcol, vec2i tile_id, tiles::types type)
{
    static std::minstd_rand rng;

    if(tile_id.x() == 0 && tile_id.y() == 0)
        return clamp(rand_det_s(rng, 0.7, 1.3) * rcol, 0, 1);

    return rcol;
}

void playspace_manager::generate_move_information()
{
    for(auto& i : entities)
    {
        i.second.cached_dijkstras = dijkstras(*this, i.second.tilemap_pos, i.second.model.get_move_distance());
    }
}

void playspace_manager::create_level(vec2i dim, level_info::types type)
{
    static std::minstd_rand rng;

    level_type = type;

    sf::Image spritemap_unprocessed;
    spritemap_unprocessed.loadFromFile("./assets/colored_transparent.png");

    constexpr vec3f transparency_colour = srgb_to_lin_approx((vec3f){71, 45, 60} / 255.f);

    for(int y=0; y < (int)spritemap_unprocessed.getSize().y; y++)
    {
        for(int x=0; x < (int)spritemap_unprocessed.getSize().x; x++)
        {
            if(y < 0 || x < 0 || x >= TILE_PIX || y >= TILE_PIX)
                continue;

            sf::Color col = spritemap_unprocessed.getPixel(x, y);

            vec4f pcol = {col.r, col.g, col.b, col.a};
            pcol = pcol / 255.f;

            vec3f to_mix = srgb_to_lin_approx(pcol.xyz());

            vec3f result = mix(to_mix, transparency_colour, 1 - pcol.w());

            pcol = lin_to_srgb_approx((vec4f){result.x(), result.y(), result.z(), 1});

            pcol = pcol * 255.f;

            pcol = round(pcol);
            pcol = clamp(pcol, 0, 255);

            spritemap_unprocessed.setPixel(x, y, sf::Color(pcol.x(), pcol.y(), pcol.z(), pcol.w()));
        }
    }

    spritemap.loadFromImage(spritemap_unprocessed);
    //spritemap.loadFromFile("./assets/colored_transparent.png");
    //spritemap.setSrgb(true);

    std::cout << "SXDIM " << spritemap.getSize().x << std::endl;

    level_size = dim;
    all_tiles.resize(level_size.x() * level_size.y());

    auto tiles = get_locations();

    for(int y=0; y < level_size.y(); y++)
    {
        for(int x=0; x < level_size.x(); x++)
        {
            auto render_type = tiles::BASE;

            renderable_object robj;
            robj.tile_id = get_tile_of(render_type);
            robj.lin_colour = get_colour_of(render_type, type);

            robj.lin_colour = clamp(rand_det_s(rng, 0.7, 1.3) * robj.lin_colour, 0, 1);

            tile_object obj;
            obj.obj = robj;

            all_tiles[y * level_size.x() + x].push_back(obj);
        }
    }

    for(auto& i : all_tiles)
    {
        if(rand_det_s(rng, 0, 1) < 0.2)
        {
            renderable_object filler;
            filler.tile_id = get_tile_of(tiles::GRASS);
            filler.lin_colour = get_colour_of(tiles::GRASS, type);

            tile_object obj;
            obj.obj = filler;

            i.push_back(obj);
        }
    }

    if(level_size.x() > 10 && level_size.y() > 10)
    {
        /*renderable_object robj;
        robj.tile_id = get_tile_of(tiles::GROUND_BUG);
        robj.lin_colour = get_colour_of(tiles::GROUND_BUG, type);

        tile_object obj;
        obj.obj = robj;

        all_tiles[10 * level_size.x() + 10].push_back(obj);*/

        auto e_1 = add_entity({10, 10}, tiles::GROUND_BUG, ai_disposition::HOSTILE, default_alien_model());
        auto e_2 = add_entity({12, 10}, tiles::GROUND_BUG, ai_disposition::HOSTILE, default_alien_model());
        auto e_3 = add_entity({13, 13}, tiles::GROUND_BUG, ai_disposition::HOSTILE, default_alien_model());

        make_squad({e_1, e_2, e_3});

        creature_model my_model = default_trooper_model();
        my_model.add_item(default_rifle());

        auto p_1 = add_entity({16, 15}, tiles::SOLDIER, ai_disposition::NONE, my_model);
    }

    generate_move_information();
}

unit_command enemy_step_single(playspace_manager& playspace, entity_object& to_step)
{
    unit_command idle_move;
    idle_move.type = unit_command::MOVE;

    //std::optional move_path = playspace.a_star(to_step.tilemap_pos, to_step.tilemap_pos + (vec2i){5, 0});

    std::optional move_path = playspace.pathfind(to_step.tilemap_pos, to_step.tilemap_pos + (vec2i){5, 0}, to_step.model.get_move_distance());

    //idle_move.move_destination = to_step.tilemap_pos + (vec2i){5, 0};
    idle_move.unit_id = to_step.my_id;

    if(move_path == std::nullopt)
    {
        idle_move.type = unit_command::END;
    }
    else
    {
        idle_move.move_path = move_path.value();
    }

    return idle_move;
}

std::optional<unit_command> enemy_step(playspace_manager& playspace)
{
    int next_exec = playspace.next_entity;
    int cur_exec = 0;

    int start_size = playspace.entities.size();

    //for(auto& i : entity_object)
    for(auto it = playspace.entities.begin(); it != playspace.entities.end(); it++)
    {
        if(it->second.disposition == ai_disposition::NONE)
            continue;

        if(next_exec == cur_exec)
        {
            return enemy_step_single(playspace, it->second);
        }

        cur_exec++;
    }

    if(start_size != (int)playspace.entities.size())
    {
        printf("Warning, playspace entities incorrectly modified\n");
    }

    return std::nullopt;
}

std::optional<unit_command> player_step(playspace_manager& playspace)
{


    return std::nullopt;
}

///ignores item effects
float get_terrainwise_hit_probability(const entity_object& source, const entity_object& target, float weapon_shoot_distance)
{
    if(weapon_shoot_distance < 1)
        return 0;

    float hit_probability_at_max = 0.1;
    float hit_probability_at_min = 1;

    vec2f diff = (vec2f){source.tilemap_pos.x(), source.tilemap_pos.y()} - (vec2f){target.tilemap_pos.x(), target.tilemap_pos.y()};

    float len = diff.length();

    if(len <= weapon_shoot_distance)
    {
        ///so. at 0 len, we hit 1, and len = weapon_shoot_distance, we hit max = 0.1
        return mix(hit_probability_at_min, hit_probability_at_max, len / weapon_shoot_distance);
    }

    return 0.f;
}

///needs to handle terrain
void render_hit_probabilities(playspace_manager& play, entity_object& eobject, item& weapon)
{
    float weapon_shoot_distance = 1;

    if(auto weapon_opt = weapon.get_facet(item_facet::RANGE); weapon_opt.has_value())
    {
        std::cout << "facet\n";

        weapon_shoot_distance = weapon_opt.value().value;
    }

    if(weapon_shoot_distance < 1)
        return;

    std::cout << "weapon " << weapon_shoot_distance << std::endl;
    std::cout << "facets " << weapon.facets.size() << std::endl;
    std::cout << "name? " << weapon.name << std::endl;

    vec2i tile_pos = eobject.tilemap_pos;

    vec2i min_pos = tile_pos - (vec2i){weapon_shoot_distance, weapon_shoot_distance};
    vec2i max_pos = tile_pos + (vec2i){weapon_shoot_distance, weapon_shoot_distance};

    min_pos = clamp(min_pos, (vec2i){0,0}, play.level_size - (vec2i){1, 1});
    max_pos = clamp(max_pos, (vec2i){0,0}, play.level_size - (vec2i){1, 1});

    for(int y=min_pos.y(); y <= max_pos.y(); y++)
    {
        for(int x=min_pos.x(); x <= max_pos.x(); x++)
        {
            vec2i target_pos = {x, y};

            int idx = target_pos.y() * play.level_size.x() + target_pos.x();

            for(const tile_object& tobj : play.all_tiles.at(idx))
            {
                if(!tobj.entity_id.has_value())
                    continue;

                const entity_object& target_entity = play.entities[tobj.entity_id.value()];

                float hit_probability = get_terrainwise_hit_probability(eobject, target_entity, weapon_shoot_distance);

                play.render_text_at(format_to_string(hit_probability, 1), play.tile_to_screen(target_entity.tilemap_pos, play.screen_dimensions), 0.f);
            }
        }
    }
}

void playspace_manager::tick(vec2f mpos, vec2f screen_dimensions, double dt_s)
{
    vec2f dir;

    if(ImGui::IsKeyDown(GLFW_KEY_W))
        dir.y() -= 1;
    if(ImGui::IsKeyDown(GLFW_KEY_S))
        dir.y() += 1;
    if(ImGui::IsKeyDown(GLFW_KEY_D))
        dir.x() += 1;
    if(ImGui::IsKeyDown(GLFW_KEY_A))
        dir.x() -= 1;

    bool can_click = !ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemHovered();

    camera_pos += dir * (dt_s * 1000);

    bool is_in_hostile_turn = (turn % 2) == 1;
    bool is_in_my_turn = (turn % 2) == 0;

    if(is_in_hostile_turn && step_enemies)
    {
        auto found = enemy_step(*this);

        step_enemies = false;

        next_entity++;

        if(found == std::nullopt)
        {
            is_in_hostile_turn = false;

            next_turn();
        }

        playing_move = found;
    }

    if(!player_building_move.has_value())
    {
        auto tile_opt = screen_to_tile(mpos, screen_dimensions);

        if(tile_opt != std::nullopt)
        {
            vec2i spos = tile_opt.value();

            //printf("FOUND TILE %i %i\n", spos.x(), spos.y());

            if(ImGui::IsMouseClicked(0, false) && can_click)
            {
                selected_tile = spos;
            }
        }
    }

    if(ImGui::IsMouseClicked(1, false) && can_click)
    {
        selected_tile = std::nullopt;
        player_building_move = std::nullopt;
    }

    if(ImGui::IsKeyPressed(GLFW_KEY_ESCAPE, false))
    {
        selected_tile = std::nullopt;
        player_building_move = std::nullopt;
    }

    if(selected_tile.has_value() && is_in_my_turn)
    {
        for(const tile_object& obj : all_tiles[selected_tile.value().y() * level_size.x() + selected_tile.value().x()])
        {
            if(!obj.entity_id.has_value())
                continue;

            entity_object& eobj = entities[obj.entity_id.value()];

            if(eobj.disposition != ai_disposition::NONE)
            {
                ImGui::Begin("Enemy Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

                eobj.model.append_rendering();

                ImGui::End();
            }
            else
            {
                ImGui::Begin("Creature Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

                eobj.model.append_rendering();

                if(ImGui::Button("Move"))
                {
                    unit_command command;
                    command.type = unit_command::MOVE;
                    command.unit_id = obj.entity_id.value();

                    player_building_move = command;
                }

                creature_model& cmodel = eobj.model;

                for(int idx = 0; idx < (int)cmodel.inventory.size(); idx++)
                {
                    item& it = cmodel.inventory[idx];

                    if(!it.get_facet(item_facet::SHOOTABLE).has_value())
                        continue;

                    ImGui::Text(it.name.c_str());

                    ImGui::SameLine();

                    if(ImGui::Button("Shoot"))
                    {
                        unit_command command;
                        command.type = unit_command::SHOOT;
                        command.unit_id = obj.entity_id.value();
                        command.item_use_id = idx;

                        player_building_move = command;
                    }
                }

                ImGui::End();
            }
        }
    }

    if(player_building_move.has_value())
    {
        unit_command& val = player_building_move.value();

        if(val.type == unit_command::SHOOT)
        {
            render_hit_probabilities(*this, entities[val.unit_id], entities[val.unit_id].model.inventory.at(val.item_use_id));
        }

        auto mpos_opt = screen_to_tile(mpos, screen_dimensions);

        if(mpos_opt.has_value())
        {
            if(ImGui::IsMouseClicked(0) && can_click)
            {
                if(val.type == unit_command::MOVE)
                {
                    const entity_object& obj = entities[val.unit_id];

                    dijkstras_info inf = dijkstras(*this, obj.tilemap_pos, obj.model.get_move_distance());

                    if(inf.get_path_cost_to(mpos_opt.value()) <= obj.model.get_move_distance())
                    {
                        std::optional<std::vector<vec2i>> path = inf.reconstruct_path(mpos_opt.value());

                        if(path.has_value())
                        {
                            val.move_path = path.value();
                            val.update_focus = true;
                            playing_move = val;
                            player_building_move = std::nullopt;
                        }
                    }
                }

                ///need to do visibility probability stuff
                if(val.type == unit_command::SHOOT)
                {
                    entity_object& firer_obj = entities[val.unit_id];
                    item& firing_item = firer_obj.model.inventory.at(val.item_use_id);

                    int tile_idx = mpos_opt.value().y() * level_size.x() + mpos_opt.value().x();

                    bool found = false;

                    for(tile_object& tobj : all_tiles.at(tile_idx))
                    {
                        if(tobj.entity_id.has_value())
                        {
                            entity_object& targeted_obj = entities[tobj.entity_id.value()];

                            handle_attack(firer_obj.model, targeted_obj.model, firing_item);
                            found = true;
                            break;
                        }
                    }

                    if(found)
                    {
                        player_building_move = std::nullopt;
                    }
                }
            }
        }
    }

    if(!is_in_hostile_turn)
    {
        player_step(*this);
    }

    #define TIME_PER_TILE_MOVED 0.1

    if(playing_move.has_value())
    {
        unit_command& current = playing_move.value();
        current.elapsed_time_s += dt_s;

        if(current.type == unit_command::END)
        {
            if(is_in_hostile_turn)
            {
                step_enemies = true;
            }

            playing_move = std::nullopt;

            generate_move_information();
        }
        else if(current.type == unit_command::MOVE)
        {
            if(current.move_path.size() == 0)
            {
                current.type = unit_command::END;
            }
            else if(current.elapsed_time_s >= TIME_PER_TILE_MOVED)
            {
                ///just move it towards destination, a* later
                /*vec2i istart = entities[current.unit_id].tilemap_pos;
                vec2i iend = clamp(current.move_destination, (vec2i){0, 0}, level_size);

                vec2i idiff = iend - istart;

                vec2f norm = (vec2f){idiff.x(), idiff.y()}.norm();

                if(norm.largest_elem() <= 0)
                    throw std::runtime_error("Bad largest elem");

                norm = norm / norm.largest_elem();*/

                assert(current.move_path.size() > 0);

                vec2i next_pos = current.move_path[0];

                current.move_path.erase(current.move_path.begin());

                move_entity_to(entities[current.unit_id], next_pos);

                current.elapsed_time_s -= TIME_PER_TILE_MOVED;

                if(current.update_focus)
                {
                    selected_tile = next_pos;
                }

                //entities[current.unit_id].tilemap_pos += (vec2i){norm.x(), norm.y()};
            }
        }
    }
}

void playspace_manager::next_turn()
{
    ///uuh who knows
    turn++;

    for(auto& i : entities)
    {
        i.second.finished_turn = false;
    }

    if((turn % 2) == 1)
    {
        step_enemies = true;
        next_entity = 0;
    }

    player_building_move = std::nullopt;
    generate_move_information();
}

void playspace_manager::draw(sf::RenderTarget& win, vec2f mpos)
{
    std::vector<sf::Vertex> vertices;
    vertices.reserve(level_size.y() * level_size.x() * 6);

    vec2f window_half_dim = {win.getSize().x/2, win.getSize().y/2};

    vec2f tl_visible = camera_pos - window_half_dim;
    vec2f br_visible = camera_pos + window_half_dim;

    int x_start = floor(tl_visible.x() / TILE_PIX) - 1;
    int y_start = floor(tl_visible.y() / TILE_PIX) - 1;

    int x_end = ceil(br_visible.x() / TILE_PIX) + 1;
    int y_end = ceil(br_visible.y() / TILE_PIX) + 1;

    x_start = clamp(x_start, 0, level_size.x());
    x_end = clamp(x_end, 0, level_size.x());
    y_start = clamp(y_start, 0, level_size.y());
    y_end = clamp(y_end, 0, level_size.y());

    auto mouse_tile_opt = screen_to_tile(mpos, {win.getSize().x, win.getSize().y});

    for(int y=y_start; y < level_size.y() && y < y_end; y++)
    {
        for(int x=x_start; x < level_size.x() && x < x_end; x++)
        {
            bool depress = false;

            if((mouse_tile_opt.has_value() && mouse_tile_opt == (vec2i){x, y}) ||
               (selected_tile.has_value() && selected_tile == (vec2i){x, y}))
            {
                depress = true;
            }

            const std::vector<tile_object>& renderables = all_tiles.at(y * level_size.x() + x);

            for(int id = 0; id < (int)renderables.size(); id++)
            {
                const tile_object& tobj = renderables[id];

                const renderable_object& renderable = tobj.obj;

                float brightness = 1;

                if(id > 0 && id != (int)renderables.size() - 1 && renderables.size() > 2)
                {
                    brightness = 0.3;
                }

                vec2f logical_pos = (vec2f){x, y} * TILE_PIX;
                vec2f real_pos = logical_pos - camera_pos;

                real_pos += window_half_dim;

                //real_pos = round(real_pos);

                vec2f tl = real_pos + (vec2f){-TILE_PIX/2, -TILE_PIX/2};
                vec2f tr = real_pos + (vec2f){TILE_PIX/2, -TILE_PIX/2};
                vec2f br = real_pos + (vec2f){TILE_PIX/2, TILE_PIX/2};
                vec2f bl = real_pos + (vec2f){-TILE_PIX/2, TILE_PIX/2};

                #ifdef DEPRESS
                ///me_irl
                constexpr float depression_constant_px = -3;

                if(depress)
                {
                    tl.y() += depression_constant_px;
                    tr.y() += depression_constant_px;
                    br.y() += depression_constant_px;
                    bl.y() += depression_constant_px;
                }
                #endif // DEPRESS

                vec4f base_colour = renderable.lin_colour;

                #define HIGHLIGHT
                #ifdef HIGHLIGHT
                if(depress && id == 0)
                {
                    base_colour = {1,1,1,1};
                }
                #endif // HIGHLIGHT

                vec2i texture_coordinate = renderable.tile_id * (TILE_PIX + TILE_SEP);

                vec2f tltx = {texture_coordinate.x(), texture_coordinate.y()};
                vec2f trtx = {texture_coordinate.x() + TILE_PIX, texture_coordinate.y()};
                vec2f brtx = {texture_coordinate.x() + TILE_PIX, texture_coordinate.y() + TILE_PIX};
                vec2f bltx = {texture_coordinate.x(), texture_coordinate.y() + TILE_PIX};

                float shade = 0.05;

                ///this is wrong because its not handling alpha correctly
                vec4f tl_col = lin_to_srgb_approx(clamp(base_colour*(1 + shade) * brightness, 0, 1));
                vec4f tr_col = lin_to_srgb_approx(base_colour * brightness);
                vec4f br_col = lin_to_srgb_approx(clamp(base_colour*(1 - shade) * brightness, 0, 1));
                vec4f bl_col = lin_to_srgb_approx(base_colour * brightness);

                sf::Color sfcol_tl(tl_col.x() * 255, tl_col.y() * 255, tl_col.z() * 255, tl_col.w() * 255);
                sf::Color sfcol_tr(tr_col.x() * 255, tr_col.y() * 255, tr_col.z() * 255, tr_col.w() * 255);
                sf::Color sfcol_br(br_col.x() * 255, br_col.y() * 255, br_col.z() * 255, br_col.w() * 255);
                sf::Color sfcol_bl(bl_col.x() * 255, bl_col.y() * 255, bl_col.z() * 255, bl_col.w() * 255);

                vertices.push_back(sf::Vertex({tl.x(), tl.y()}, sfcol_tl, {tltx.x(), tltx.y()}));
                vertices.push_back(sf::Vertex({bl.x(), bl.y()}, sfcol_bl, {bltx.x(), bltx.y()}));
                vertices.push_back(sf::Vertex({br.x(), br.y()}, sfcol_br, {brtx.x(), brtx.y()}));

                vertices.push_back(sf::Vertex({tl.x(), tl.y()}, sfcol_tl, {tltx.x(), tltx.y()}));
                vertices.push_back(sf::Vertex({br.x(), br.y()}, sfcol_br, {brtx.x(), brtx.y()}));
                vertices.push_back(sf::Vertex({tr.x(), tr.y()}, sfcol_tr, {trtx.x(), trtx.y()}));
            }
        }
    }

    if(vertices.size() > 0)
    {
        sf::RenderStates states;
        states.blendMode = sf::BlendAlpha;
        states.texture = &spritemap;

        win.draw(&vertices[0], vertices.size(), sf::PrimitiveType::Triangles, states);

        if(selected_tile.has_value() && !playing_move.has_value())
        {
            for(tile_object& tile : all_tiles[selected_tile.value().y() * level_size.x() + selected_tile.value().x()])
            {
                if(!tile.entity_id.has_value())
                    continue;

                sf::RectangleShape shape;
                shape.setSize({TILE_PIX - 4, TILE_PIX - 4});
                shape.setOrigin({TILE_PIX/2 - 2, TILE_PIX/2 - 2});
                shape.setFillColor(sf::Color(0,0,0,0));
                shape.setOutlineColor(sf::Color(255, 255, 255, 60));
                shape.setOutlineThickness(1);

                entity_object& entity = entities[tile.entity_id.value()];

                const std::vector<std::pair<vec2i, float>>& dijkstra_info = entity.cached_dijkstras.path_costs;

                for(auto& i : dijkstra_info)
                {
                    if(i.second > entity.model.get_move_distance())
                        continue;

                    vec2i pos = i.first;

                    vec2f rpos = tile_to_screen(pos, window_half_dim*2);

                    shape.setPosition(rpos.x(), rpos.y());
                    win.draw(shape, states);
                }
            }
        }
    }

    auto render_renderable = [&](async_renderable& renderable, int idx)
    {
        //auto last_pos = ImGui::GetCursorPos();

        if(renderable.type == async_renderable::TEXT)
        {
            vec2f spos = {renderable.screen_pos.x() + ImGui::GetMainViewport()->Pos.x, renderable.screen_pos.y() + ImGui::GetMainViewport()->Pos.y};

            ImGui::SetNextWindowPos({spos.x(), spos.y()});

            std::string id_str = "Testo##text" + std::to_string(idx);

            ImGui::Begin(id_str.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoInputs);

            //ImGui::Begin(id_str.c_str());

            //ImGui::SetCursorScreenPos({renderable.screen_pos.x(), renderable.screen_pos.y()});

            /*ImGui::Text("Hello\n");*/

            ImGui::Text(renderable.text_info.c_str());

            //ImGui::GetWindowDrawList()->AddText({spos.x(), spos.y()}, 0xFFFFFFFF, renderable.text_info.c_str(), nullptr);

            ImGui::End();
        }

        //ImGui::SetCursorPos(last_pos);
    };

    for(int i=0; i < (int)async_renderables.size(); i++)
    {
        render_renderable(async_renderables[i], i);

        if((async_renderables[i].elapsed.getElapsedTime().asMicroseconds() / 1000. / 1000.) > async_renderables[i].timeout_s)
        {
            async_renderables.erase(async_renderables.begin() + i);
            i--;
            continue;
        }
    }
}

uint64_t playspace_manager::add_entity(vec2i where, tiles::types type, ai_disposition::types ai_type, const creature_model& model)
{
    if(where.x() < 0 || where.y() < 0 || where.x() >= level_size.x() || where.y() >= level_size.y())
        throw std::runtime_error("No! Bad add entity at " + std::to_string(where.x()) + " " + std::to_string(where.y()));

    renderable_object robj;
    robj.tile_id = get_tile_of(type);
    robj.lin_colour = get_colour_of(type, level_type);

    tile_object obj;
    obj.obj = robj;
    obj.passable = false;

    entity_object eobj;
    eobj.tilemap_pos = where;
    eobj.disposition = ai_type;
    eobj.model = model;

    eobj.my_id = entity_gid++;
    obj.entity_id = eobj.my_id;

    all_tiles[where.y() * level_size.x() + where.x()].push_back(obj);

    entities[eobj.my_id] = eobj;

    return eobj.my_id;
}

void playspace_manager::make_squad(const std::vector<uint64_t>& ids)
{
    auto nid = squad_gid++;

    for(auto& i : ids)
    {
        entities[i].squad_id = nid;
    }
}

void playspace_manager::move_entity_to(entity_object& object, vec2i destination)
{
    vec2i start = object.tilemap_pos;

    if(start.x() < 0 || start.y() < 0 || start.x() >= level_size.x() || start.y() >= level_size.y())
        throw std::runtime_error("OOB\n");

    if(destination.x() < 0 || destination.y() < 0 || destination.x() >= level_size.x() || destination.y() >= level_size.y())
        throw std::runtime_error("OOB2\n");

    auto& tile_list = all_tiles[start.y() * level_size.x() + start.x()];

    std::optional<tile_object> found;

    for(int i=0; i < (int)tile_list.size(); i++)
    {
        if(tile_list[i].entity_id.has_value() && tile_list[i].entity_id.value() == object.my_id)
        {
            found = tile_list[i];

            tile_list.erase(tile_list.begin() + i);
            break;
        }
    }

    if(found == std::nullopt)
        throw std::runtime_error("Nullopt in move_entity_to");

    object.tilemap_pos = destination;
    all_tiles[destination.y() * level_size.x() + destination.x()].push_back(found.value());
}

std::optional<std::vector<vec2i>> playspace_manager::pathfind(vec2i start, vec2i finish, float max_path_cost)
{
    return dijkstras(*this, start, max_path_cost).reconstruct_path(finish);
}

std::optional<vec2i> playspace_manager::screen_to_tile(vec2f screen_pos, vec2f screen_dimensions)
{
    vec2f relative = screen_pos - screen_dimensions/2.f + camera_pos;

    vec2f tile_coord = round(relative / (float)TILE_PIX);

    vec2i as_int = (vec2i){tile_coord.x(), tile_coord.y()};

    if(as_int.x() < 0 || as_int.y() < 0 || as_int.x() >= level_size.x() || as_int.y() >= level_size.y())
        return std::nullopt;

    return as_int;
}

vec2f playspace_manager::tile_to_screen(vec2i tile_pos, vec2f screen_dimensions)
{
    vec2f relative = (vec2f){tile_pos.x(), tile_pos.y()} * TILE_PIX - camera_pos + screen_dimensions/2.f;

    return relative;
}

void playspace_manager::render_text_at(const std::string& text, vec2f screen_pos, float timeout_s)
{
    async_renderable renderable;
    renderable.type = async_renderable::TEXT;
    renderable.text_info = text;
    renderable.timeout_s = timeout_s;
    renderable.screen_pos = screen_pos;

    async_renderables.push_back(renderable);
}
