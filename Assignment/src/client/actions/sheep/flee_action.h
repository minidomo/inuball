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

    Array find_nearby_goals();
    Vector3 determine_target_direction(Array goals);
    float find_max_goal_speed(Array goals);

   public:
    static void _register_methods();

    void _init();

    virtual void initialize() override;
    virtual void tick(real_t delta) override;
};

#endif