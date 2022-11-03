#include "./base_chicken_action.h"

#include "../../debug.h"
#include "../../objects/animals/chicken.h"

void BaseChickenAction::_register_methods() {}

Chicken *BaseChickenAction::get_base_typed() {
    return Object::cast_to<Chicken>(base);
}
