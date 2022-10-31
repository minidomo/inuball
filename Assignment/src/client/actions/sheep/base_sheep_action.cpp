#include "./base_sheep_action.h"

#include "../../debug.h"
#include "../../objects/animals/sheep.h"

void BaseSheepAction::_register_methods() {}

Sheep *BaseSheepAction::get_base_typed() {
    return Object::cast_to<Sheep>(base);
}
