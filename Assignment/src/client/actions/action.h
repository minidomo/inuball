#ifndef A4_ACTION_H
#define A4_ACTION_H

#include <Node.hpp>

#include "../common.h"

class Action : public Node {
    GODOT_CLASS(Action, Node);

   protected:
    Node *base;

   public:
    static void _register_methods();

    void _init();

    void set_base(Node *base);
    Node *get_base();

    /**
     * Perform any initial steps prior to this action starting
     */
    virtual void initialize();

    /**
     * Should be called in _physics_process() and performs the action
     */
    virtual void tick(real_t delta);
};

#endif