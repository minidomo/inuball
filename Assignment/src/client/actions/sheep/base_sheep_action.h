#ifndef A4_BASE_SHEEP_ACTION_H
#define A4_BASE_SHEEP_ACTION_H

#include <Node.hpp>

#include "../../common.h"
#include "../base.h"

class Sheep;

class BaseSheepAction : public Node, public Base<Sheep> {
    GODOT_CLASS(BaseSheepAction, Node);

   public:
    static void _register_methods();

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