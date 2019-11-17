#ifndef COMBAT_MODEL_HPP_INCLUDED
#define COMBAT_MODEL_HPP_INCLUDED

#include <vec/vec.hpp>

namespace body_part_info
{
    enum types
    {
        ARM,
        LEG,
        HEAD,
        TORSO,
        COUNT
    };
}

namespace combat_info
{
    enum hit_types
    {
        DIRECT,
        GRAZE,
        MISS,
    };
}

struct body_part
{
    float hp = 1;
    float max_hp = 1;
    float base_hit_probability = 1; ///100%
    body_part_info::types type = body_part_info::COUNT;

    float get_functionality();
    combat_info::hit_types roll_to_hit(std::minstd_rand& gen, float probability_modification);
    void hit(float damage);
};

struct creature_model
{
    std::vector<body_part> parts;

    combat_info::hit_types hit_random_bodypart_for(float damage, float probability_modification);
    float get_move_speed_fraction();
    float get_aim_accuracy_fraction();
};

creature_model default_alien_model();
creature_model default_trooper_model();

#endif // COMBAT_MODEL_HPP_INCLUDED
