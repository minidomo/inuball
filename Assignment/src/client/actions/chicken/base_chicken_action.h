#ifndef A4_BASE_CHICKEN_ACTION_H
#define A4_BASE_CHICKEN_ACTION_H

#include "../../common.h"
#include "../action.h"

class Chicken;

class BaseChickenAction : public Action {
    GODOT_CLASS(BaseChickenAction, Action);

   public:
    static void _register_methods();

    Chicken *get_base_typed();
};

#endif