#include "./flee_action.h"

#include "../../objects/animals/sheep.h"
#include "../../objects/goal.h"

void FleeAction::_register_methods() {
    register_property<FleeAction, float>("min_flee_speed",
                                         &FleeAction::min_flee_speed, 5);
    register_property<FleeAction, float>("max_flee_speed",
                                         &FleeAction::max_flee_speed, 15);
    register_property<FleeAction, float>(
        "flee_speed_multiplier", &FleeAction::flee_speed_multiplier, 1.5f);
}

void FleeAction::_init() {
    min_flee_speed = 5;
    max_flee_speed = 15;
    flee_speed_multiplier = 1.5f;
}

void FleeAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": flee action init");
}

void FleeAction::tick(real_t delta) {
    Array goals = find_nearby_goals();

    if (goals.empty()) return;

    Sheep *sheep = get_base_typed();

    // determine the direction to move in and the speed
    Vector3 target_direction = determine_target_direction(goals);
    float max_speed = find_max_goal_speed(goals);
    Vector3 velocity = target_direction * max_speed;

    // apply gravity
    if (!sheep->is_on_floor()) {
        velocity.y -= sheep->gravity * delta;
    }

    sheep->velocity =
        sheep->move_and_slide_with_snap(velocity, Vector3::DOWN, Vector3::UP);

    sheep->look_forward();
}

Array FleeAction::find_nearby_goals() {
    Sheep *sheep = get_base_typed();
    Array bodies = sheep->area->get_overlapping_bodies();

    Array goals;

    for (int i = 0; i < bodies.size(); i++) {
        Goal *goal = Object::cast_to<Goal>(bodies[i]);

        if (goal) {
            goals.append(goal);
        }
    }

    return goals;
}

Vector3 FleeAction::determine_target_direction(Array goals) {
    Vector3 sheep_origin = get_base_typed()->get_global_transform().origin;
    Vector3 net_origin;

    for (int i = 0; i < goals.size(); i++) {
        Goal *goal = Object::cast_to<Goal>(goals[i]);
        net_origin += goal->get_global_transform().origin;
    }

    Vector3 net_direction = net_origin.direction_to(sheep_origin);

    net_direction.y = 0;
    net_direction.normalize();

    return net_direction;
}

float FleeAction::find_max_goal_speed(Array goals) {
    float max_speed = min_flee_speed;

    for (int i = 0; i < goals.size(); i++) {
        Goal *goal = Object::cast_to<Goal>(goals[i]);

        Vector3 velocity = goal->get_linear_velocity();
        velocity.y = 0;

        max_speed = Math::max<float>(max_speed, velocity.length());
    }

    max_speed = Math::min(max_speed * flee_speed_multiplier, max_flee_speed);

    return max_speed;
}
