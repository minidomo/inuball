#include "./base_sheep_action.h"

#include "../../debug.h"
#include "../../objects/animals/sheep.h"

void BaseSheepAction::_register_methods() {}

Sheep *BaseSheepAction::get_base_typed() {
    return Object::cast_to<Sheep>(base);
}

void BaseSheepAction::tick(real_t delta) {
    Sheep *sheep = get_base_typed();

    // copied physics process for now as default behavior for wander
    if (!sheep->is_on_floor()) {
        sheep->velocity.y -= sheep->gravity * delta;
    }

    sheep->velocity = sheep->move_and_slide_with_snap(
        sheep->velocity, Vector3::DOWN, Vector3::UP);

    Vector3 orientation = sheep->velocity;
    orientation.y = 0;
    orientation.normalize();
    orientation.rotate(Vector3::UP, Math::deg2rad(90.0f));

    if (orientation != Vector3::ZERO) {
        Vector3 origin = sheep->get_global_transform().origin;

        // the difference between target and origin needs to be large enough
        // so look_at works
        float multiplier = 50;
        Vector3 target = origin + orientation * multiplier;

        sheep->look_at(target, Vector3::UP);
    }
}
