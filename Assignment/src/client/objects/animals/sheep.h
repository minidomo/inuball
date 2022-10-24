
#ifndef SHEEP_H
#define SHEEP_H

#include <InputEvent.hpp>

#include "animal.h"

class Sheep : public Animal<Animals::SHEEP> {
    GODOT_CLASS(Sheep, Animal)
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
    void _process(real_t t) { Animal::_process(t); }
    void _physics_process(real_t t) { Animal::_physics_process(t); }

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
};

#endif
