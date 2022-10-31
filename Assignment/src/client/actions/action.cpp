#include "./action.h"

#include "../debug.h"

void Action::_register_methods() {
    register_method("_ready", &Action::_ready);
    register_method("initialize", &Action::initialize);
    register_method("tick", &Action::tick);
}

void Action::_ready() { Godot::print("action is ready"); }

void Action::set_base(Node *base) { this->base = base; }

Node *Action::get_base() { return base; }

void Action::initialize() {
    // this function should be overridden
    dev_assert(false);
}

void Action::tick(real_t delta) {
    // this function should be overridden
    dev_assert(false);
}