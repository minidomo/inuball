#include "./flock_action.h"

#include "../../objects/animals/chicken.h"

void FlockAction::_register_methods() {}

void FlockAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": flock action init");
}

void FlockAction::tick(real_t delta) {}