#include "./base_sheep_action.h"

#include "../../debug.h"
#include "../../objects/animals/sheep.h"

void BaseSheepAction::_register_methods() {
    register_method("initialize", &BaseSheepAction::initialize);
    register_method("tick", &BaseSheepAction::tick);
}

void BaseSheepAction::initialize() {
    // this function should be overridden
    dev_assert(false);
}

void BaseSheepAction::tick(real_t delta) {
    // this function should be overridden
    dev_assert(false);
}