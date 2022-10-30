#include "./attack_action.h"

#include "../../objects/animals/sheep.h"

AttackAction::AttackAction(Sheep *sheep) { this->sheep = sheep; }

void AttackAction::init() { Godot::print("attack action init"); }

void AttackAction::tick(real_t delta) {}