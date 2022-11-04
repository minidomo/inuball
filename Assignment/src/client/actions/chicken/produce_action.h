#ifndef A4_PRODUCE_ACTION_H
#define A4_PRODUCE_ACTION_H

#include "../../common.h"
#include "./base_chicken_action.h"

class ProduceAction : public BaseChickenAction {
    GODOT_CLASS(ProduceAction, BaseChickenAction);

   public:
    static void _register_methods();

    virtual void initialize() override;
    virtual Vector3 get_collide_pos();
    virtual void allow_breed();
    virtual void try_breed();
    virtual void tick(real_t delta) override;
};

#endif