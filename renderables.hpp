#ifndef RENDERABLES_HPP_INCLUDED
#define RENDERABLES_HPP_INCLUDED

#include <vec/vec.hpp>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <optional>
#include "pathfinding.hpp"
#include "combat_model.hpp"

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
    std::optional<uint64_t> entity_id;
};

struct entity_object
{
    vec2i tilemap_pos;
    uint64_t my_id = -1;
    bool finished_turn = false;
    size_t squad_id = -1;
    creature_model model;

    ai_disposition::types disposition = ai_disposition::NONE;

    dijkstras_info cached_dijkstras;
};

struct unit_command
{
    enum types
    {
        MOVE,
        SHOOT,
        END,
    };

    uint64_t unit_id = -1;
    std::vector<vec2i> move_path;
    types type = types::END;
    double elapsed_time_s = 0;
    bool update_focus = false;

    int item_use_id = -1;
};

struct async_renderable
{
    enum types
    {
        TEXT,
        NONE
    };

    vec2f screen_pos = {0,0};
    std::string text_info;

    types type = types::NONE;
    float timeout_s = 0; ///0 means render once then vanish
    sf::Clock elapsed;

    static inline int gid = 0;
    int id = gid++;
};

struct playspace_manager
{
    sf::Texture spritemap;

    static inline uint64_t entity_gid = 0;
    static inline uint64_t squad_gid = 0;

    vec2f camera_pos;
    vec2f screen_dimensions;
    float zoom = 1;

    vec2i level_size = {0,0};
    std::vector<std::vector<tile_object>> all_tiles;
    std::map<uint64_t, entity_object> entities; ///UNITS, not anything else. Actionable entities like chickens, crocodiles

    std::vector<async_renderable> async_renderables;

    level_info::types level_type = level_info::GRASS;

    void generate_move_information();
    void create_level(vec2i dim, level_info::types type);

    void tick(vec2f mpos, vec2f screen_dimensions, double dt_s);
    void next_turn();
    void draw(sf::RenderTarget& win, vec2f mpos);

    uint64_t turn = 0;
    bool step_enemies = false;
    int next_entity = 0;

    uint64_t add_entity(vec2i where, tiles::types type, ai_disposition::types ai_type, const creature_model& model);
    void make_squad(const std::vector<uint64_t>& ids);

    std::optional<unit_command> playing_move;
    std::optional<unit_command> player_building_move;

    void move_entity_to(entity_object& object, vec2i destination);

    std::optional<std::vector<vec2i>> pathfind(vec2i start, vec2i finish, float max_path_cost);
    std::optional<vec2i> screen_to_tile(vec2f screen_pos, vec2f screen_dimensions);
    vec2f tile_to_screen(vec2i tile, vec2f screen_dimensions);

    std::optional<vec2i> selected_tile;

    void render_text_at(const std::string& text, vec2f screen_pos, float timeout_s = 0);
};

#endif // RENDERABLES_HPP_INCLUDED
