#include "./flee_action.h"

void FleeAction::_register_methods() {
    register_method("initialize", &FleeAction::initialize);
    register_method("tick", &FleeAction::tick);
}

void FleeAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": flee action init");
}

void FleeAction::tick(real_t delta) {}