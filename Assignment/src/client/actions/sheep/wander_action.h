#ifndef A4_WANDER_ACTION_H
#define A4_WANDER_ACTION_H

#include <OpenSimplexNoise.hpp>

#include "../../common.h"
#include "./base_sheep_action.h"

class WanderAction : public BaseSheepAction {
    GODOT_CLASS(WanderAction, BaseSheepAction);

   private:
    OpenSimplexNoise *noise = nullptr;
    real_t timer = 0.0f;
    real_t prev_noise = 0.0f;

   public:
    static void _register_methods();

    virtual void initialize() override;
    virtual void tick(real_t delta) override;

    void _init() {
        noise = OpenSimplexNoise::_new();
        noise->set_seed((int64_t)(void *)this);
    }
};

#endif