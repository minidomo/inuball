#ifndef A4_FLOCK_ACTION_H
#define A4_FLOCK_ACTION_H

#include "../../common.h"
#include "./base_chicken_action.h"

class FlockAction : public BaseChickenAction {
    GODOT_CLASS(FlockAction, BaseChickenAction);

    float MAX_SPEED;
    float VISION;

    float ALIGN_FORCE;
    float COHESION_FORCE;
    float SEPERATION_FORCE;

    Vector3 accel;

   public:
    static void _register_methods();

    virtual Vector3 align(Array neighbors);
    virtual Vector3 cohesion(Array neighbors);
    virtual Vector3 seperation(Array neighbors);

    virtual Vector3 steer(Vector3 tgt);
    virtual Array get_neighbors();

    virtual void initialize() override;
    virtual void tick(real_t delta) override;
};

#endif