#ifndef A4_FLEE_ACTION_H
#define A4_FLEE_ACTION_H

#include "../../common.h"
#include "./base_sheep_action.h"

class FleeAction : public BaseSheepAction {
    GODOT_CLASS(FleeAction, BaseSheepAction);

   private:
    float min_flee_speed;
    float max_flee_speed;
    float flee_speed_multiplier;
    float max_wall_distance;

    /**
     * an array of dictionaries each with the following structure:
     * normal: Vector3
     * position: Vector3
     */
    Array collided_walls;

    Array find_nearby_goals();
    Vector3 determine_target_direction(Array goals);
    float find_max_goal_speed(Array goals);

    void update_collided_walls();
    void remove_far_walls();

    static float measure_normal_distance(Dictionary wall, Vector3 point);
    static int find_wall(Array walls, Vector3 normal);
    static Vector3 sum_normals(Array walls);

   public:
    static void _register_methods();

    void _init();

    virtual void initialize() override;
    virtual void tick(real_t delta) override;
};

#endif