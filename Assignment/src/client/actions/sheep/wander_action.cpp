#include "./wander_action.h"

void WanderAction::_register_methods() {
    register_method("initialize", &WanderAction::initialize);
    register_method("tick", &WanderAction::tick);
}

void WanderAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": wander action init");
}

void WanderAction::tick(real_t delta) {}