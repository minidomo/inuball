#ifndef A4_ATTACK_ACTION_H
#define A4_ATTACK_ACTION_H

#include "../../common.h"
#include "./base_sheep_action.h"

class AttackAction : public BaseSheepAction {
    GODOT_CLASS(AttackAction, BaseSheepAction);

   public:
    static void _register_methods();

    virtual void initialize() override;
    virtual void tick(real_t delta) override;
};

#endif