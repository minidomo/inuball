#ifndef A4_WANDER_ACTION_H
#define A4_WANDER_ACTION_H

#include "../../common.h"
#include "./base_sheep_action.h"

class WanderAction : public BaseSheepAction {
    GODOT_CLASS(WanderAction, Node);

   public:
    static void _register_methods();

    virtual void initialize() override;
    virtual void tick(real_t delta) override;
};

#endif