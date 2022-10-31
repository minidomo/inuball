#ifndef A4_BASE_SHEEP_ACTION_H
#define A4_BASE_SHEEP_ACTION_H

#include "../../common.h"
#include "../action.h"

class Sheep;

class BaseSheepAction : public Action {
    GODOT_CLASS(BaseSheepAction, Action);

   public:
    static void _register_methods();

    Sheep *get_base_typed();
};

#endif