#include "./wander_action.h"

#include "../../objects/animals/sheep.h"

void WanderAction::_register_methods() {}

void WanderAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": wander action init ");
    timer = 0.0f;
}

void WanderAction::tick(real_t delta) {
    BaseSheepAction::tick(delta);
    Sheep *sheep = get_base_typed();

    if (!sheep->is_on_floor()) {
        return;
    }

    auto norm3 = sheep->get_floor_normal();

    Vector2 vel(sheep->velocity.x, sheep->velocity.z);
    Vector2 norm(norm3.x, norm3.z);

    prev_noise = noise->get_noise_2d(timer, prev_noise);

    auto rot = (prev_noise + 1) * Math_PI;

    vel = (vel * 0.9 + 0.1 * norm.rotated(rot));

    vel.normalize();
    vel *= 5;

    sheep->velocity.x = vel.x;
    sheep->velocity.z = vel.y;

    timer += delta;
}