#ifndef PATHFINDING_HPP_INCLUDED
#define PATHFINDING_HPP_INCLUDED

#include <vector>
#include <map>
#include <vec/vec.hpp>

struct dijkstras_info
{
    vec2i min_pos;
    vec2i max_pos;
    std::vector<std::pair<vec2i, float>> path_costs;
    std::map<vec2i, vec2i> before;
    vec2i start;

    std::optional<std::vector<vec2i>> reconstruct_path(vec2i finish);
    float get_path_cost_to(vec2i finish);
};

struct playspace_manager;

//std::optional<std::vector<vec2i>> a_star(playspace_manager& play, vec2i first, vec2i finish);

///returns accessible positions
dijkstras_info dijkstras(playspace_manager& play, vec2i start, float max_cost);

#endif // PATHFINDING_HPP_INCLUDED
