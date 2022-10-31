#include "./attack_action.h"

#include "../../objects/animals/sheep.h"

void AttackAction::_register_methods() {}

void AttackAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": attack action init");
}

void AttackAction::tick(real_t delta) {}