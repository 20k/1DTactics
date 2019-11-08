#ifndef PATHFINDING_HPP_INCLUDED
#define PATHFINDING_HPP_INCLUDED

#include <vector>
#include <vec/vec.hpp>

struct playspace_manager;

std::optional<std::vector<vec2i>> a_star(playspace_manager& play, vec2i first, vec2i finish);

///returns accessible positions
std::vector<std::pair<vec2i, float>> dijkstras(playspace_manager& play, vec2i start, float max_cost);

#endif // PATHFINDING_HPP_INCLUDED
