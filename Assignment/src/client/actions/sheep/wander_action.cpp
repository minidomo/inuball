#include "./wander_action.h"

#include "../../objects/animals/sheep.h"

void WanderAction::_register_methods() {}

void WanderAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": wander action init ");
}

void WanderAction::tick(real_t delta) {
    Sheep *sheep = get_base_typed();

    // copied physics process for now as default behavior for wander
    if (!sheep->is_on_floor()) {
        sheep->velocity.y -= sheep->gravity * delta;
    }

    sheep->velocity = sheep->move_and_slide_with_snap(
        sheep->velocity, Vector3::DOWN, Vector3::UP);

    sheep->look_forward();
}