#include "./action.h"

#include "../debug.h"

void Action::_register_methods() {}

void Action::_init() {}

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