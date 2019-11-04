#ifndef RENDERABLES_HPP_INCLUDED
#define RENDERABLES_HPP_INCLUDED

#include <vec/vec.hpp>
#include <vector>

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
    add_to(ret[DIRT], {0, 0});
    add_to(ret[DIRT], {1, 0});
    add_to(ret[DIRT], {2, 0});
    add_to(ret[DIRT], {3, 0});
    add_to(ret[DIRT], {4, 0});

    add_to(ret[GRASS], {0, 0});
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

    ///top left man is 23, 0

    add_to(ret[CIVILIAN], {24, 0});
    add_to(ret[CIVILIAN], {25, 0});
    add_to(ret[CIVILIAN], {25, 1});
    add_to(ret[CIVILIAN], {26, 1});
    add_to(ret[CIVILIAN], {28, 1});
    add_to(ret[CIVILIAN], {29, 1});
    add_to(ret[CIVILIAN], {29, 3});
    add_to(ret[CIVILIAN], {30, 3});
    add_to(ret[CIVILIAN], {29, 4});
    add_to(ret[CIVILIAN], {30, 4});
    ///top left scorpion is 23, 5

    add_to(ret[SOLDIER], {25, 4});
    add_to(ret[SOLDIER], {29, 9});

    add_to(ret[GROUND_BUG], {27, 5});
    add_to(ret[GROUND_BUG], {28, 5});
    add_to(ret[GROUND_BUG], {29, 5});
    add_to(ret[SMALL_PINCHY], {30, 5});
    add_to(ret[FLYING_BUG], {25, 5});
    add_to(ret[ARMOURED_BUG], {26, 5});
    add_to(ret[SCORPION], {23, 5});

    add_to(ret[LAND_ANIMAL], {24, 7});
    add_to(ret[LAND_ANIMAL], {25, 7});
    add_to(ret[LAND_ANIMAL], {26, 7});
    add_to(ret[LAND_ANIMAL], {27, 7});
    //add_to(ret[LAND_ANIMAL], {28, 7});
    add_to(ret[LAND_ANIMAL], {29, 7});
    add_to(ret[LAND_ANIMAL], {30, 7});
    add_to(ret[LAND_ANIMAL], {25, 8}); //bat

    add_to(ret[SEA_ANIMAL], {24, 8});
    add_to(ret[SEA_ANIMAL], {27, 8});
    add_to(ret[CROCODILE], {28, 8});

    add_to(ret[FACE_MALE], {23, 10});
    add_to(ret[FACE_MALE], {25, 10});
    add_to(ret[FACE_MALE], {26, 10});
    add_to(ret[FACE_MALE], {27, 10});
    add_to(ret[FACE_MALE], {28, 10});

    add_to(ret[FACE_WOMAN], {24, 10});
    add_to(ret[FACE_WOMAN], {29, 10});
    add_to(ret[FACE_WOMAN], {30, 10});

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

struct entity_manager
{

};


#endif // RENDERABLES_HPP_INCLUDED
