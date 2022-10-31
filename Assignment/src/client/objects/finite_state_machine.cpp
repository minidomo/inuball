#include "./finite_state_machine.h"

#include "../debug.h"

void FiniteStateMachine::_register_methods() {
    register_method("_ready", &FiniteStateMachine::_ready);

    register_property<FiniteStateMachine, int>(
        "max_num_states", &FiniteStateMachine::max_num_states, 0);
}

void FiniteStateMachine::_init() {
    state = 0;
    current_action = nullptr;
    max_num_states = 0;
}

void FiniteStateMachine::_ready() {
    dev_assert(max_num_states >= 0);
    dev_assert(get_child_count() == max_num_states);
}

void FiniteStateMachine::perform_action(real_t delta) {
    current_action->tick(delta);
}

void FiniteStateMachine::update_state(int state) {
    dev_assert(state >= 0 && state < max_num_states);

    this->state = state;
    current_action = get_action(state);
    current_action->initialize();
}

int FiniteStateMachine::get_state() { return state; }

Action* FiniteStateMachine::get_action(int i) {
    Action* action = Object::cast_to<Action>(get_child(i));
    dev_assert(action != nullptr);
    return action;
}
