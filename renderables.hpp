#ifndef RENDERABLES_HPP_INCLUDED
#define RENDERABLES_HPP_INCLUDED

#include <vec/vec.hpp>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#define TILE_PIX 16
#define TILE_SEP 1

namespace tiles
{
    enum types
    {
        BASE,
        DIRT,
        GRASS,
        TREE_1,
        TREE_2,
        TREE_DENSE,
        TREE_ROUND,
        CACTUS,
        VINE,
        SHRUB,
        ROCKS,
        BRAMBLE,
        CIVILIAN,
        SOLDIER,
        GROUND_BUG,
        FLYING_BUG,
        ARMOURED_BUG,
        SCORPION,
        SMALL_PINCHY,
        LAND_ANIMAL,
        SEA_ANIMAL,
        CROCODILE,
        FACE_MALE,
        FACE_WOMAN,
        THIN_DOOR_CLOSED,
        THIN_DOOR_OPEN,
        DOOR_CLOSED,
        DOOR_OPEN,
        GRAVE,
        WOOD_FENCE_FULL,
        WOOD_FENCE_HALF,
        TILING_WALL,
        CULTIVATION,
    };
}

namespace ai_disposition
{
    enum types
    {
        HOSTILE, ///the bugs
        FRIENDLY, ///not sure
        IDLES, ///animals
        NONE
    };
}

namespace level_info
{
    enum types
    {
        BARREN, ///dirt, some grass
        DESERT,
        GRASS
    };
}

///in 1bit spritemap
std::map<tiles::types, std::vector<vec2i>>& get_locations();

struct renderable_object
{
    vec2i tile_id = {0,0};
    vec4f lin_colour = {1,1,1,1};
};

struct tile_object
{
    renderable_object obj;
    float path_cost = 1;
    bool passable = true;
};

struct entity_object
{
    vec2i tilemap_pos;
    vec2i next_pos;

    ai_disposition::types disposition = ai_disposition::NONE;
};

struct playspace_manager
{
    sf::Texture spritemap;

    vec2f camera_pos;
    float zoom = 1;

    vec2i level_size = {0,0};
    std::vector<std::vector<tile_object>> all_tiles;
    std::vector<entity_object> entities;
    level_info::types level_type = level_info::GRASS;

    void create_level(vec2i dim, level_info::types type);

    void tick(double dt_s);
    void next_turn();
    void draw(sf::RenderTarget& win);

    uint64_t turn = 0;

    entity_object& add_entity(vec2i where, tiles::types type, ai_disposition::types ai_type);
};

#endif // RENDERABLES_HPP_INCLUDED
