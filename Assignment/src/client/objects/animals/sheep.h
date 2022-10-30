
#ifndef SHEEP_H
#define SHEEP_H

#include <Area.hpp>
#include <InputEvent.hpp>
#include <memory>
#include <vector>

#include "../../actions/sheep/attack_action.h"
#include "../../actions/sheep/flee_action.h"
#include "../../actions/sheep/wander_action.h"
#include "../enums/sheep_state.h"
#include "animal.h"

class Sheep : public Animal<Animals::SHEEP> {
    GODOT_CLASS(Sheep, Animal)

   private:
    Area *area;
    int state;
    Action *action;
    vector<unique_ptr<Action>> actions;

   public:
    static void _register_methods();

    void _init() {
        Animal::_init();
        state = +SheepState::DEFAULT;
    }

    void _ready() {
        LookingAtReceiver::subscribe(this);
        interactStream =
            Object::cast_to<AudioStreamPlayer>(get_node("InteractSound"));
        deathStream =
            Object::cast_to<AudioStreamPlayer>(get_node("DeathSound"));

        area = Object::cast_to<Area>(get_node("Area"));
        area->connect("body_entered", this, "on_body_entered");
        area->connect("body_exited", this, "on_body_exited");

        // set actions for fsm
        actions.resize(4);

        actions[+SheepState::DEFAULT].reset(new WanderAction(this));
        actions[+SheepState::NEAR_PLAYER].reset(new AttackAction(this));

        FleeAction *flee_action = new FleeAction(this);
        actions[+SheepState::NEAR_GOAL].reset(flee_action);
        actions[+SheepState::NEAR_GOAL | +SheepState::NEAR_PLAYER].reset(
            flee_action);

        update_action();
        action->init();
    }

    void _process(real_t t) { Animal::_process(t); }

    void _physics_process(real_t delta);

    void _input(Ref<InputEvent> event);

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
    int check_state();
};

#endif
