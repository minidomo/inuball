#ifndef A4_FINITE_STATE_MACHINE_H
#define A4_FINITE_STATE_MACHINE_H

#include <Node.hpp>

#include "../actions/action.h"
#include "../common.h"

class FiniteStateMachine : public Node {
    GODOT_CLASS(FiniteStateMachine, Node);

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
};

#endif