#ifndef A4_FLOCK_ACTION_H
#define A4_FLOCK_ACTION_H

#include "../../common.h"
#include "./base_chicken_action.h"

class FlockAction : public BaseChickenAction {
    GODOT_CLASS(FlockAction, BaseChickenAction);

   public:
    static void _register_methods();

    virtual void initialize() override;
    virtual void tick(real_t delta) override;
};

#endif