#ifndef A4_SCATTER_ACTION_H
#define A4_SCATTER_ACTION_H

#include "../../common.h"
#include "./base_chicken_action.h"

class ScatterAction : public BaseChickenAction {
    GODOT_CLASS(ScatterAction, BaseChickenAction);

   public:
    static void _register_methods();

    virtual void initialize() override;
    virtual void tick(real_t delta) override;
};

#endif