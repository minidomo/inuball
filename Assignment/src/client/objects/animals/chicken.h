
#ifndef CHICKEN_H
#define CHICKEN_H

#include <InputEvent.hpp>

#include "animal.h"

class Chicken : public Animal<Animals::CHICKEN> {
    GODOT_CLASS(Chicken, Animal)
   public:
    static void _register_methods();

    void _init() { Animal::_init(); }
    void _ready() {
        LookingAtReceiver::subscribe(this);
        interactStream =
            Object::cast_to<AudioStreamPlayer>(get_node("InteractSound"));
        deathStream =
            Object::cast_to<AudioStreamPlayer>(get_node("DeathSound"));
    }
    void _input(Ref<InputEvent> event);

    void _process(real_t t) { Animal::_process(t); }
    void _physics_process(real_t t) { Animal::_physics_process(t); }

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

    virtual void collide_with_player(Player *player) {
        Animal::collide_with_player(player);
    }

    virtual void entered_goal(Goal *goal) override;
};

#endif
