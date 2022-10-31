#ifndef A4_FLEE_ACTION_H
#define A4_FLEE_ACTION_H

#include "../../common.h"
#include "./base_sheep_action.h"

class FleeAction : public BaseSheepAction {
    GODOT_CLASS(FleeAction, BaseSheepAction);

   public:
    static void _register_methods();

    virtual void initialize() override;
    virtual void tick(real_t delta) override;
};

#endif