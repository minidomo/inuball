#include "./flee_action.h"

#include "../../objects/animals/sheep.h"

FleeAction::FleeAction(Sheep *sheep) { this->sheep = sheep; }

void FleeAction::init() { Godot::print("flee action init"); }

void FleeAction::tick(real_t delta) {}