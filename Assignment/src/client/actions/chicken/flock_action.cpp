#include "./flock_action.h"

#include "../../objects/animals/chicken.h"

void FlockAction::_register_methods() {}

void FlockAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": flock action init");

    MAX_SPEED = 15.0;
    VISION = 5.0;

    ALIGN_FORCE = 0.6;
    COHESION_FORCE = 0.6;
    SEPERATION_FORCE = 1.0;

    accel = Vector3(0.f, 0.f, 0.f);

    Chicken *chicken = get_base_typed();
    chicken->velocity = Vector3((rand() / (float)RAND_MAX - .5) * MAX_SPEED, 0,
                                (rand() / (float)RAND_MAX - .5) * MAX_SPEED);
}

Vector3 FlockAction::steer(Vector3 tgt) {
    Chicken *chicken = get_base_typed();
    Vector3 steer = tgt - chicken->velocity;
    return steer;
}

Array FlockAction::get_neighbors() {
    Array ret = Array();
    Chicken *chicken = get_base_typed();
    Array chickens = chicken->get_chickens();
    for (int i = 0; i < chickens.size(); i++) {
        Chicken *cur = Object::cast_to<Chicken>(chickens[i]);
        if (chicken != cur && chicken->get_translation().distance_to(
                                  cur->get_translation()) < VISION) {
            ret.push_back(cur);
        }
    }
    return ret;
}

Vector3 FlockAction::align(Array neighbors) {
    Chicken *chicken = get_base_typed();
    Vector3 res(0.f, 0.f, 0.f);
    if (neighbors.size() == 0) {
        return res;
    }
    for (int i = 0; i < neighbors.size(); i++) {
        res += Object::cast_to<Chicken>(neighbors[i])->velocity;
    }
    res /= neighbors.size();
    return steer((res - chicken->get_translation()).normalized() * MAX_SPEED);
}

Vector3 FlockAction::cohesion(Array neighbors) {
    Chicken *chicken = get_base_typed();
    Vector3 res(0.f, 0.f, 0.f);
    if (neighbors.size() == 0) {
        return res;
    }
    for (int i = 0; i < neighbors.size(); i++) {
        res += Object::cast_to<Chicken>(neighbors[i])->get_translation();
    }
    res /= neighbors.size();
    return steer(res.normalized() * MAX_SPEED);
}

Vector3 FlockAction::seperation(Array neighbors) {
    Chicken *chicken = get_base_typed();
    Array closer_n = Array();
    Vector3 res(0.f, 0.f, 0.f);
    for (int i = 0; i < neighbors.size(); i++) {
        Chicken *cur = Object::cast_to<Chicken>(neighbors[i]);
        float dist =
            chicken->get_translation().distance_to(cur->get_translation());
        if (dist < VISION / 2) {
            closer_n.push_back(cur);
        }
    }
    if (closer_n.size() == 0) {
        return res;
    }
    for (int i = 0; i < closer_n.size(); i++) {
        Chicken *cur = Object::cast_to<Chicken>(closer_n[i]);
        Vector3 diff = chicken->get_translation() - cur->get_translation();
        res += diff.normalized() / diff.length();
    }
    res /= closer_n.size();
    return steer(res.normalized() * MAX_SPEED);
}

// void FlockAction::tick(real_t delta) {
//     Chicken *chicken = get_base_typed();
//     Array neighbors = FlockAction::get_neighbors();

//     accel += FlockAction::align(neighbors) * ALIGN_FORCE;
//     accel += FlockAction::cohesion(neighbors) * COHESION_FORCE;
//     accel += FlockAction::seperation(neighbors) * SEPERATION_FORCE;
//     accel = Vector3(accel.x, 0, accel.z);

//     chicken->velocity += accel * delta;
//     chicken->velocity = chicken->velocity.normalized() * MAX_SPEED;

//     if (!chicken->is_on_floor()) {
//         chicken->velocity.y -= chicken->gravity * delta;
//     }

//     chicken->velocity = chicken->move_and_slide_with_snap(
//         chicken->velocity, Vector3::DOWN, Vector3::UP);
// }

void FlockAction::tick(real_t delta) {
    Chicken *sheep = get_base_typed();

    // copied physics process for now as default behavior for wander
    if (!sheep->is_on_floor()) {
        sheep->velocity.y -= sheep->gravity / 10.0 * delta;
    }

    sheep->velocity = sheep->move_and_slide_with_snap(
        sheep->velocity, Vector3::DOWN, Vector3::UP);
}