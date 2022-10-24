
#include "chicken.h"

#include <InputEventKey.hpp>

using namespace godot;

void Chicken::_register_methods() {
    register_method("_process", &Chicken::_process);
    register_method("_phyiscs_process", &Chicken::_physics_process);
    register_method("_ready", &Chicken::_ready);
    register_method("_input", &Chicken::_input);
    register_method("handleLookAt", &Chicken::handleLookAt);
    register_method("entered_goal", &Chicken::entered_goal);
}

void Chicken::handleLookAt(Node *player, Node *target, Vector3 point,
                           Vector3 normal, real_t distance) {
    Animal::handleLookAt(player, target, point, normal, distance);
}

void Chicken::_input(Ref<InputEvent> event) {
    InputEventKey *key_event = Object::cast_to<InputEventKey>(event.ptr());
    if (key_event) {
        int64_t key = key_event->get_scancode();
        if (key == GlobalConstants::KEY_U && key_event->is_pressed()) {
            sfxDb += sfxSens;
            interactStream->set_volume_db(sfxDb);
            deathStream->set_volume_db(sfxDb);
        } else if (key == GlobalConstants::KEY_J && key_event->is_pressed()) {
            sfxDb -= sfxSens;
            interactStream->set_volume_db(sfxDb);
            deathStream->set_volume_db(sfxDb);
        } else if (key == GlobalConstants::KEY_P && key_event->is_pressed()) {
            interactStream->set_stream_paused(
                !interactStream->get_stream_paused());
            deathStream->set_stream_paused(!deathStream->get_stream_paused());
        }
    }
}

void Chicken::entered_goal(Goal *goal) {
    Animal::entered_goal(goal);
    goal->score("Chicken", 5, 1);
}