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

struct playspace_manager
{
    sf::Texture spritemap;

    vec2f camera_pos;
    float zoom = 1;

    vec2i level_size = {0,0};
    std::vector<tile_object> all_tiles;

    void create_level(vec2i dim, level_info::types type);

    void tick(double dt_s);
    void draw(sf::RenderWindow& win);
};

#endif // RENDERABLES_HPP_INCLUDED
