
#include "sheep.h"

#include <InputEventKey.hpp>

using namespace godot;

void Sheep::_register_methods() {
    register_method("_process", &Sheep::_process);
    register_method("_physics_process", &Sheep::_physics_process);
    register_method("_ready", &Sheep::_ready);
    register_method("_input", &Sheep::_input);
    register_method("handleLookAt", &Sheep::handleLookAt);
    register_method("entered_goal", &Sheep::entered_goal);

    register_property<Sheep, float>("Gravity", &Sheep::gravity, 50.0);
}

void Sheep::_ready() {
    LookingAtReceiver::subscribe(this);
    interactStream =
        Object::cast_to<AudioStreamPlayer>(get_node("InteractSound"));
    deathStream = Object::cast_to<AudioStreamPlayer>(get_node("DeathSound"));

    area = Object::cast_to<Area>(get_node("Area"));

    fsm = Object::cast_to<FiniteStateMachine>(get_node("FiniteStateMachine"));
    fsm->setup(this, +SheepState::DEFAULT);
}

void Sheep::handleLookAt(Node *player, Node *target, Vector3 point,
                         Vector3 normal, real_t distance) {
    Animal::handleLookAt(player, target, point, normal, distance);
}

void Sheep::_input(Ref<InputEvent> event) {
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

void Sheep::entered_goal(Goal *goal) {
    Animal::entered_goal(goal);
    goal->score("Sheep", 1, 1);
}

void Sheep::_physics_process(real_t delta) {
    int updated_state = compute_state();

    if (updated_state != fsm->get_state()) {
        fsm->update_state(updated_state);
    }

    fsm->perform_action(delta);
}

int Sheep::compute_state() {
    int state = 0;

    Array bodies = area->get_overlapping_bodies();
    for (int i = 0; i < bodies.size(); i++) {
        Goal *goal = Object::cast_to<Goal>(bodies[i]);
        Player *player = Object::cast_to<Player>(bodies[i]);

        if (goal) {
            state |= +SheepState::NEAR_GOAL;
        } else if (player) {
            state |= +SheepState::NEAR_PLAYER;
        }
    }

    return state;
}
