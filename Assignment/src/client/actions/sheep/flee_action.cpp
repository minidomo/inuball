#include "./flee_action.h"

#include <KinematicCollision.hpp>
#include <Ref.hpp>

#include "../../objects/animals/sheep.h"
#include "../../objects/goal.h"

void FleeAction::_register_methods() {
    register_property<FleeAction, float>("min_flee_speed",
                                         &FleeAction::min_flee_speed, 5);
    register_property<FleeAction, float>("max_flee_speed",
                                         &FleeAction::max_flee_speed, 15);
    register_property<FleeAction, float>(
        "flee_speed_multiplier", &FleeAction::flee_speed_multiplier, 1.5f);
    register_property<FleeAction, float>("max_wall_distance",
                                         &FleeAction::max_wall_distance, 20);
}

void FleeAction::_init() {
    min_flee_speed = 5;
    max_flee_speed = 15;
    flee_speed_multiplier = 1.5f;
    max_wall_distance = 20;
}

void FleeAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": flee action init");
}

void FleeAction::tick(real_t delta) {
    /*
    flee works such that the sheep will move away from the goal(s) in the
    direction corresponding to the sheep's and goal's position.

    to handle cases where a sheep will be colliding into a wall, or the world
    border in our case, we will maintain an array containing a wall's normal and
    position. we will account the wall's normal into the direction calculation
    as long as the sheep is within a certain distance to the wall which is
    determined by `max_wall_distance`.

    ultimately, this will help prevent the sheep from switching back and forth
    from fleeing into or off a wall. furthermore, it essentially results in the
    sheep to flee the wall once it touches a wall.
    */

    BaseSheepAction::tick(delta);
    Array goals = find_nearby_goals();

    if (goals.empty()) return;

    Sheep *sheep = get_base_typed();

    update_collided_walls();

    // determine the direction to move in and the speed
    Vector3 target_direction = determine_target_direction(goals);
    float max_speed = find_max_goal_speed(goals);

    // maintain y velocity and change everything else
    float vel_y = sheep->velocity.y;
    sheep->velocity = target_direction * max_speed;
    sheep->velocity.y = vel_y;
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
    Vector3 wall_normals = sum_normals(collided_walls);
    net_direction += wall_normals;

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

void FleeAction::update_collided_walls() {
    Sheep *sheep = get_base_typed();

    remove_far_walls();

    for (int i = 0; i < sheep->get_slide_count(); i++) {
        Ref<KinematicCollision> collision = sheep->get_slide_collision(i);
        Vector3 velocity = collision->get_collider_velocity();
        Vector3 normal = collision->get_normal();

        if (velocity == Vector3::ZERO && normal.max_axis() != Vector3::AXIS_Y) {
            Vector3 position = collision->get_position();

            Dictionary data;
            data["normal"] = normal;
            data["position"] = position;

            int index = find_wall(collided_walls, normal);
            if (index == -1) {
                collided_walls.append(data);
            } else {
                collided_walls[index] = data;
            }
        }
    }
}

void FleeAction::remove_far_walls() {
    Sheep *sheep = get_base_typed();
    int size = collided_walls.size();

    for (int i = size - 1; i >= 0; i--) {
        Dictionary data = collided_walls[i];
        Vector3 origin = sheep->get_global_transform().origin;

        float distance = measure_normal_distance(data, origin);
        if (distance > max_wall_distance) {
            collided_walls.remove(i);
        }
    }
}

float FleeAction::measure_normal_distance(Dictionary wall, Vector3 point) {
    Vector3 normal = wall["normal"];
    Vector3 position = wall["position"];

    // the distance measured should be a straight line from the wall to the
    // point so we need to adjust the wall's position which depends on its
    // normal
    if (normal.min_axis() == Vector3::AXIS_Z) {
        position.z = point.z;
    } else {
        position.x = point.x;
    }

    position.y = point.y;

    return position.distance_to(point);
}

int FleeAction::find_wall(Array walls, Vector3 normal) {
    for (int i = 0; i < walls.size(); i++) {
        Dictionary data = walls[i];
        Vector3 wall_normal = data["normal"];

        if (wall_normal == normal) {
            return i;
        }
    }

    return -1;
}

Vector3 FleeAction::sum_normals(Array walls) {
    Vector3 sum;

    for (int i = 0; i < walls.size(); i++) {
        Dictionary data = walls[i];
        Vector3 normal = data["normal"];
        sum += normal;
    }

    return sum;
}