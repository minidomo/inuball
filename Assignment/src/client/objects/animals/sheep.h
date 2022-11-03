
#ifndef SHEEP_H
#define SHEEP_H

#include <Area.hpp>
#include <InputEvent.hpp>

#include "../../actions/sheep/attack_action.h"
#include "../../actions/sheep/base_sheep_action.h"
#include "../../actions/sheep/flee_action.h"
#include "../../actions/sheep/wander_action.h"
#include "../enums/sheep_state.h"
#include "../finite_state_machine.h"
#include "animal.h"

class Sheep : public Animal<Animals::SHEEP> {
    GODOT_CLASS(Sheep, Animal);

    friend class BaseSheepAction;
    friend class AttackAction;
    friend class FleeAction;
    friend class WanderAction;

   private:
    Area *area;
    FiniteStateMachine *fsm;
    Player *chargingAt = nullptr;

   public:
    static void _register_methods();

    void _init() { Animal::_init(); }

    void _ready();

    void _process(real_t t) { Animal::_process(t); }

    void _physics_process(real_t delta);

    void _input(Ref<InputEvent> event);

    virtual void collide_with_player(Player *player) {
        Animal::collide_with_player(player);
    }

    virtual void handleLookAt(Node *player, Node *target, Vector3 point,
                              Vector3 normal, real_t distance) override;
    virtual bool interact_secondary(Node *player) override {
        Animal::interact_secondary(player);
        interactStream->play();
        return true;
    }

    virtual bool interact_primary(Node *player) override {
        Animal::interact_primary(player);
        deathStream->play();  // TODO: Play entirely before queue_free
        return true;
    }

    virtual void entered_goal(Goal *goal) override;

    void on_body_entered(Node *body);
    void on_body_exited(Node *body);

    void update_action();
    int compute_state();
};

#endif
