#include "pathfinding.hpp"
#include <deque>
#include <set>
#include "renderables.hpp"

template<typename T>
std::vector<T> reconstruct_path(std::map<T, T>& came_from, const T& current)
{
    T internal = current;

    std::vector<T> total_path{internal};

    while(came_from[internal] != T())
    {
        internal = came_from[internal];

        total_path.push_back(internal);
    }

    std::reverse(total_path.begin(), total_path.end());

    return total_path;
}

template<typename T>
float heuristic(const T& start, const T& fin)
{
    return (start - fin).length();
}

template<typename T>
std::optional<T> any_of_a_in_b(const std::vector<T>& a, const std::vector<T>& b)
{
    for(auto& i : a)
    {
        for(auto& j : b)
        {
            if(i == j)
                return i;
        }
    }

    return std::nullopt;
}

///this function was originally designed for an arbitrary linked graph and could be made significantly faster
template<typename T>
std::vector<T> get_shortest_path(playspace_manager& play, const std::vector<T>& start_vec, const std::vector<T>& fin_vec, int cap = -1)
{
    std::vector<T> ret;

    if(fin_vec.size() == 0)
        return ret;

    if(auto val = any_of_a_in_b(start_vec, fin_vec); val.has_value())
    {
        return {*val};
    }

    std::set<T> closed_set;
    std::vector<T> open_set = start_vec;

    std::map<T, T> came_from;

    std::map<T, float> g_score;

    for(auto& i : start_vec)
    {
        g_score[i] = 0.f;
    }

    std::deque<std::pair<T, float>> sorted;

    for(auto& i : start_vec)
    {
        sorted.push_back(std::pair(i, 0.f));
    }

    int num_explored = 0;

    while(sorted.size() > 0)
    {
        T current_sys = std::move(sorted[0].first);

        sorted.pop_front();

        for(auto& i : fin_vec)
        {
            if(current_sys == i)
            {
                return reconstruct_path(came_from, current_sys);
            }
        }

        if(closed_set.find(current_sys) != closed_set.end())
            continue;

        closed_set.insert(current_sys);

        std::vector<T> flinks;
        flinks.reserve(4);

        auto check_apply = [&](vec2i offset)
        {
            if(offset.x() < 0 || offset.y() < 0 || offset.x() >= play.level_size.x() || offset.y() >= play.level_size.y())
                return;

            auto& found = play.all_tiles[offset.y() * play.level_size.x() + offset.x()];

            for(tile_object& i : found)
            {
                if(!i.passable)
                    return;
            }

            flinks.push_back(offset);
        };

        check_apply(current_sys + (vec2i){-1, 0});
        check_apply(current_sys + (vec2i){1, 0});
        check_apply(current_sys + (vec2i){0, -1});
        check_apply(current_sys + (vec2i){0, 1});

        for(const T& next_sys : flinks)
        {
            if(closed_set.find(next_sys) != closed_set.end())
                continue;

            auto looking_gscore = g_score.find(next_sys);
            float found_gscore = g_score[current_sys] + 1;

            float testing_gscore = FLT_MAX;

            if(looking_gscore != g_score.end())
            {
                testing_gscore = looking_gscore->second;
            }

            if(found_gscore >= testing_gscore)
                continue;

            came_from[next_sys] = current_sys;

            g_score[next_sys] = found_gscore;

            //sorted.push_back({next_sys, found_gscore + heuristic(ctx, next_sys, fin)});
            sorted.push_back({next_sys, found_gscore});

            for(auto& i : sorted)
            {
                if(i.first == next_sys)
                {
                    //i.second = found_gscore + heuristic(ctx, next_sys, fin);
                    i.second = found_gscore;
                    break;
                }
            }

            std::sort(sorted.begin(), sorted.begin(),
                  [](const std::pair<T, float>& p1, const std::pair<T, float>& p2){return p1.second < p2.second;});

            num_explored++;

            if(num_explored > cap && cap != -1)
                return ret;
        }
    }

    return ret;
}

std::optional<std::vector<vec2i>> a_star(playspace_manager& play, vec2i first, vec2i finish)
{
    if(first == finish)
        return {};

    auto found = get_shortest_path<vec2i>(play, {first}, {finish}, 10000);

    if(found.size() == 0)
        return std::nullopt;

    return found;
}

