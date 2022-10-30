#ifndef A4_WANDER_ACTION_H
#define A4_WANDER_ACTION_H

#include "../../common.h"
#include "../action.h"

class Sheep;

class WanderAction : public Action<Sheep> {
   public:
    using Action<Sheep>::Action;

    virtual void init();
    virtual void tick(real_t delta);
};

#endif