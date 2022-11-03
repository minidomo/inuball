#include "./attack_action.h"

#include "../../objects/animals/sheep.h"

void AttackAction::_register_methods() {}

void AttackAction::initialize() {
    String id = String::num_int64(base->get_instance_id());
    Godot::print(id + ": attack action init");
}

void AttackAction::tick(real_t delta) {
    BaseSheepAction::tick(delta);
    Sheep *sheep = get_base_typed();

    auto player = sheep->chargingAt;
    if (!player) return;

    auto diff = player->get_translation() - sheep->get_translation();
    diff.y = 0;
    auto len = sheep->velocity.length();
    sheep->velocity += diff.normalized();
    // if (len > 10) sheep->velocity = sheep->velocity.normalized() * len;
}
