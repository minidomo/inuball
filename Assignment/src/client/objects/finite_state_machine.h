#ifndef A4_FINITE_STATE_MACHINE_H
#define A4_FINITE_STATE_MACHINE_H

#include <Node.hpp>

#include "../actions/action.h"
#include "../common.h"

class FiniteStateMachine : public Node {
    GODOT_CLASS(FiniteStateMachine, Node);

    /*
    Actions should be ordered under a finite state machine machine such that its
    index in the FSM's children corresponds to its associated state.
    */

   private:
    Action *current_action;
    int state;

    int max_num_states;

    Action *get_action(int i);

   public:
    static void _register_methods();

    void _init();
    void _ready();

    void perform_action(real_t delta);

    void update_state(int state);
    int get_state();

    void set_children_base(Node *base);
    void setup(Node *base, int initial_state);
};

#endif