
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
    register_method("on_body_entered", &Sheep::on_body_entered);
    register_method("on_body_exited", &Sheep::on_body_exited);

    register_property<Sheep, float>("Gravity", &Sheep::gravity, 50.0);
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

void Sheep::on_body_entered(Node *body) {
    Goal *goal = Object::cast_to<Goal>(body);
    Player *player = Object::cast_to<Player>(body);

    String id = String::num_int64(get_instance_id());

    Array arr = area->get_overlapping_bodies();
    String max_index = String::num_int64(arr.size() - 1);

    if (goal) {
        String index = "-1";
        for (int i = 0; i < arr.size(); i++) {
            Goal *cur = Object::cast_to<Goal>(arr[i]);
            if (cur && cur->get_instance_id() == goal->get_instance_id()) {
                index = String::num_int64(i);
            }
        }

        Godot::print(id + ": goal is nearby " + index + "/" + max_index);
    } else if (player) {
        String index = "-1";
        for (int i = 0; i < arr.size(); i++) {
            Player *cur = Object::cast_to<Player>(arr[i]);
            if (cur && cur->get_instance_id() == player->get_instance_id()) {
                index = String::num_int64(i);
            }
        }

        Godot::print(id + ": player is nearby " + index + "/" + max_index);
    }
}

void Sheep::on_body_exited(Node *body) {}

void Sheep::wander() {}

void Sheep::flee() {}

void Sheep::attack() {}