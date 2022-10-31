#include "./attack_action.h"

void AttackAction::_register_methods() {
    register_method("initialize", &AttackAction::initialize);
    register_method("tick", &AttackAction::tick);
}

void AttackAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": attack action init");
}

void AttackAction::tick(real_t delta) {}