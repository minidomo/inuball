#include "./flee_action.h"

#include "../../objects/animals/sheep.h"

void FleeAction::_register_methods() {}

void FleeAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": flee action init");
}

void FleeAction::tick(real_t delta) {}