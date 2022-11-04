#include "./scatter_action.h"

#include "../../objects/animals/chicken.h"
#include "../../objects/player.h"

void ScatterAction::_register_methods() {}

void ScatterAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": hide action init");

    MAX_SPEED = 8.0;

    Chicken *chicken = get_base_typed();
    chicken->velocity = Vector3((rand() / (float)RAND_MAX - .5) * MAX_SPEED, 0,
                                (rand() / (float)RAND_MAX - .5) * MAX_SPEED);
}

Vector3 ScatterAction::get_collide_pos() {
    Chicken *chicken = get_base_typed();
    Array bodies = chicken->area->get_overlapping_bodies();
    Vector3 res(0.f, 0.f, 0.f);
    int count = 0;
    for (int i = 0; i < bodies.size(); i++) {
        Player *tmp = Object::cast_to<Player>(bodies[i]);

        if (tmp) {
            count += 1;
            res += tmp->get_translation();
        }
    }
    if (count == 0) {
        return res;
    }
    res /= count;
    return res;
}

void ScatterAction::tick(real_t delta) {
    Chicken *chicken = get_base_typed();
    Array bodies = chicken->area->get_overlapping_bodies();
    Vector3 new_vel =
        chicken->get_translation() - ScatterAction::get_collide_pos();
    if (new_vel != chicken->get_translation()) {
        new_vel = new_vel.normalized() * MAX_SPEED;
        chicken->velocity = new_vel;
    }

    if (!chicken->is_on_floor()) {
        chicken->velocity.y -= chicken->gravity * delta;
    }

    chicken->velocity = chicken->move_and_slide_with_snap(
        chicken->velocity, Vector3::DOWN, Vector3::UP);
}