#include "chicken.h"

#include <InputEventKey.hpp>

using namespace godot;

void Chicken::_register_methods() {
    register_method("_process", &Chicken::_process);
    register_method("_physics_process", &Chicken::_physics_process);
    register_method("_ready", &Chicken::_ready);
    register_method("_input", &Chicken::_input);
    register_method("set_chickens", &Chicken::set_chickens);
    register_method("get_chickens", &Chicken::get_chickens);
    register_method("handleLookAt", &Chicken::handleLookAt);
    register_method("entered_goal", &Chicken::entered_goal);
    register_method("set_state_pref", &Chicken::set_state_pref);

    register_property<Chicken, float>("Gravity", &Chicken::gravity, 50.0);
}

void Chicken::_ready() {
    LookingAtReceiver::subscribe(this);
    interactStream =
        Object::cast_to<AudioStreamPlayer>(get_node("InteractSound"));
    deathStream = Object::cast_to<AudioStreamPlayer>(get_node("DeathSound"));

    area = Object::cast_to<Area>(get_node("Area"));

    fsm = Object::cast_to<FiniteStateMachine>(get_node("FiniteStateMachine"));
    fsm->setup(this, +ChickenState::DEFAULT);
    can_breed = false;
    breed_timer = nullptr;
    state_timer = Timer()._new();
    state_timer->set_one_shot(true);
    state_timer->connect("timeout", this, "set_state_pref");
    add_child(state_timer);
    state_pref = ChickenState::PRODUCE;
    set_state_pref();
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

void Chicken::set_state_pref() {
    state_pref = state_pref == ChickenState::DEFAULT ? ChickenState::PRODUCE
                                                     : ChickenState::DEFAULT;
    state_timer->start(10.0 + (rand() / (float)RAND_MAX * 20.0));
}

void Chicken::entered_goal(Goal *goal) {
    Animal::entered_goal(goal);
    goal->score("Chicken", 5, 1);
}

void Chicken::_physics_process(real_t delta) {
    int updated_state = compute_state();

    if (updated_state != fsm->get_state()) {
        fsm->update_state(updated_state);
    }

    fsm->perform_action(delta);
}

int Chicken::compute_state() {
    int state = +state_pref;

    Array bodies = area->get_overlapping_bodies();
    for (int i = 0; i < bodies.size(); i++) {
        Player *player = Object::cast_to<Player>(bodies[i]);
        if (player) {
            state = +ChickenState::SCATTER;
        }
    }

    return state;
}