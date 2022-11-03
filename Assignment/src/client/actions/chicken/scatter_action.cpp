#include "./scatter_action.h"

#include "../../objects/animals/chicken.h"

void ScatterAction::_register_methods() {}

void ScatterAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": scatter action init");
}

void ScatterAction::tick(real_t delta) {}