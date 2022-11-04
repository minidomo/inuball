#include "./produce_action.h"

#include <Spatial.hpp>
#include <Timer.hpp>

#include "../../enums/chicken_state.h"
#include "../../objects/animals/chicken.h"

void ProduceAction::_register_methods() {
    register_method("allow_breed", &ProduceAction::allow_breed);
}

void ProduceAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": hide action init");

    Chicken *chicken = get_base_typed();
    Timer *breed_timer = Timer()._new();
    breed_timer->set_one_shot(true);
    breed_timer->set_wait_time(15);
    breed_timer->connect("timeout", this, "allow_breed");
    chicken->breed_timer = breed_timer;
    add_child(breed_timer);
    breed_timer->start();
}

void ProduceAction::allow_breed() {
    Chicken *chicken = get_base_typed();
    chicken->can_breed = true;
}

void ProduceAction::try_breed() {
    Chicken *chicken = get_base_typed();
    Array bodies = chicken->area->get_overlapping_bodies();
    for (int i = 0; i < bodies.size(); i++) {
        Chicken *tmp = Object::cast_to<Chicken>(bodies[i]);

        // How the hell is this cast not implicitly done?
        if (tmp && tmp->fsm->get_state() == (int)ChickenState::PRODUCE &&
            tmp->can_breed) {
            chicken->can_breed = false;
            tmp->can_breed = false;
            chicken->attemptbreed(tmp);
            Godot::print("tried breed!");
            chicken->breed_timer->start();
            tmp->breed_timer->start();
            break;
        }
    }
}

Vector3 ProduceAction::get_collide_pos() {
    Chicken *chicken = get_base_typed();
    Array bodies = chicken->area->get_overlapping_bodies();
    Vector3 res(0.f, 0.f, 0.f);
    int count = 0;
    for (int i = 0; i < bodies.size(); i++) {
        Spatial *tmp = Object::cast_to<Spatial>(bodies[i]);

        if (tmp) {
            count += 1;
            res += tmp->get_translation();
        }
    }
    res /= count;
    return res;
}

void ProduceAction::tick(real_t delta) {
    Chicken *chicken = get_base_typed();
    if (!chicken->is_on_floor()) {
        chicken->velocity.y -= chicken->gravity * delta;
    }

    chicken->velocity = chicken->move_and_slide_with_snap(
        chicken->velocity, Vector3::DOWN, Vector3::UP);

    Vector3 orientation = chicken->velocity;
    orientation.y = 0;
    orientation.normalize();
    orientation.rotate(Vector3::UP, Math::deg2rad(90.0f));

    if (orientation != Vector3::ZERO) {
        Vector3 origin = chicken->get_global_transform().origin;

        // the difference between target and origin needs to be large enough
        // so look_at works
        float multiplier = 50;
        Vector3 target = origin + orientation * multiplier;

        chicken->look_at(target, Vector3::UP);
    }

    if (chicken->can_breed) {
        try_breed();
    }
}