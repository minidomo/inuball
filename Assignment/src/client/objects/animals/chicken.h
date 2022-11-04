
#ifndef CHICKEN_H
#define CHICKEN_H

#include <Area.hpp>
#include <InputEvent.hpp>
#include <Timer.hpp>

#include "../../actions/chicken/base_chicken_action.h"
#include "../../actions/chicken/flock_action.h"
#include "../../actions/chicken/produce_action.h"
#include "../../actions/chicken/scatter_action.h"
#include "../enums/chicken_state.h"
#include "../finite_state_machine.h"
#include "animal.h"

class Chicken : public Animal<Animals::CHICKEN> {
    GODOT_CLASS(Chicken, Animal);

    friend class BaseChickenAction;
    friend class FlockAction;
    friend class ProduceAction;
    friend class ScatterAction;

   private:
    Area *area;
    FiniteStateMachine *fsm;
    Array chickens;
    bool can_breed;
    Timer *breed_timer;
    Timer *state_timer;
    ChickenState state_pref;

   public:
    static void _register_methods();

    void _init() { Animal::_init(); }
    void _ready();
    void _input(Ref<InputEvent> event);

    void _process(real_t t) { Animal::_process(t); }
    void _physics_process(real_t t);

    void set_chickens(Array chickens) { this->chickens = chickens; }

    void set_state_pref();

    Array get_chickens() { return this->chickens; }

    virtual void handleLookAt(Node *player, Node *target, Vector3 point,
                              Vector3 normal, real_t distance) override;
    virtual bool interact_secondary(Node *player) override {
        Animal::interact_secondary(player);
        interactStream->play();
        return true;
    }
    virtual bool interact_primary(Node *player) override {
        Animal::interact_primary(player);
        deathStream->play();
        return true;
    }

    virtual void entered_goal(Goal *goal) override;

    void update_action();
    int compute_state();
};

#endif
