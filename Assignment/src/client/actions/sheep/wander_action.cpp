#include "./wander_action.h"

#include "../../objects/animals/sheep.h"

WanderAction::WanderAction(Sheep *sheep) { this->sheep = sheep; }

void WanderAction::init() { Godot::print("wander action init"); }

void WanderAction::tick(real_t delta) {}