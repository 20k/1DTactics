#include "renderables.hpp"
#include <assert.h>
#include <iostream>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

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

        add_entity({10, 10}, tiles::GROUND_BUG, ai_disposition::HOSTILE);
    }
}

void playspace_manager::tick(double dt_s)
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

    camera_pos += dir * (dt_s * 1000);
}

void playspace_manager::next_turn()
{
    ///uuh who knows
    turn++;
}

void playspace_manager::draw(sf::RenderTarget& win)
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

    for(int y=y_start; y < level_size.y() && y < y_end; y++)
    {
        for(int x=x_start; x < level_size.x() && x < x_end; x++)
        {
            for(const tile_object& tobj : all_tiles.at(y * level_size.x() + x))
            {
                const renderable_object& renderable = tobj.obj;

                vec2f logical_pos = (vec2f){x, y} * TILE_PIX;
                vec2f real_pos = logical_pos - camera_pos;

                real_pos += window_half_dim;

                //real_pos = round(real_pos);

                vec2f tl = real_pos + (vec2f){-TILE_PIX/2, -TILE_PIX/2};
                vec2f tr = real_pos + (vec2f){TILE_PIX/2, -TILE_PIX/2};
                vec2f br = real_pos + (vec2f){TILE_PIX/2, TILE_PIX/2};
                vec2f bl = real_pos + (vec2f){-TILE_PIX/2, TILE_PIX/2};

                vec2i texture_coordinate = renderable.tile_id * (TILE_PIX + TILE_SEP);

                vec2f tltx = {texture_coordinate.x(), texture_coordinate.y()};
                vec2f trtx = {texture_coordinate.x() + TILE_PIX, texture_coordinate.y()};
                vec2f brtx = {texture_coordinate.x() + TILE_PIX, texture_coordinate.y() + TILE_PIX};
                vec2f bltx = {texture_coordinate.x(), texture_coordinate.y() + TILE_PIX};

                float shade = 0.05;

                ///this is wrong because its not handling alpha correctly
                vec4f tl_col = lin_to_srgb_approx(clamp(renderable.lin_colour*(1 + shade), 0, 1));
                vec4f tr_col = lin_to_srgb_approx(renderable.lin_colour);
                vec4f br_col = lin_to_srgb_approx(clamp(renderable.lin_colour*(1 - shade), 0, 1));
                vec4f bl_col = lin_to_srgb_approx(renderable.lin_colour);

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

    if(vertices.size() == 0)
        return;

    sf::RenderStates states;
    states.blendMode = sf::BlendAlpha;
    states.texture = &spritemap;

    win.draw(&vertices[0], vertices.size(), sf::PrimitiveType::Triangles, states);
}

entity_object& playspace_manager::add_entity(vec2i where, tiles::types type, ai_disposition::types ai_type)
{
    if(where.x() < 0 || where.y() < 0 || where.x() >= level_size.x() || where.y() >= level_size.y())
        throw std::runtime_error("No! Bad add entity at " + std::to_string(where.x()) + " " + std::to_string(where.y()));

    renderable_object robj;
    robj.tile_id = get_tile_of(type);
    robj.lin_colour = get_colour_of(type, level_type);

    tile_object obj;
    obj.obj = robj;
    obj.passable = false;

    all_tiles[where.y() * level_size.x() + where.x()].push_back(obj);

    entity_object eobj;
    eobj.tilemap_pos = where;
    eobj.next_pos = where;
    eobj.disposition = ai_type;

    eobj.my_id = entity_gid++;

    entities[eobj.my_id] = eobj;

    return entities[eobj.my_id];
}
