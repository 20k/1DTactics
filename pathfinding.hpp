#ifndef PATHFINDING_HPP_INCLUDED
#define PATHFINDING_HPP_INCLUDED

#include <vector>
#include <vec/vec.hpp>

struct playspace_manager;

std::optional<std::vector<vec2i>> a_star(playspace_manager& play, vec2i first, vec2i finish);

#endif // PATHFINDING_HPP_INCLUDED
