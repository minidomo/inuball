#ifndef A4_FLEE_ACTION_H
#define A4_FLEE_ACTION_H

#include "../../common.h"
#include "../action.h"

class Sheep;

class FleeAction : public Action<Sheep> {
   public:
    using Action<Sheep>::Action;

    virtual void init();
    virtual void tick(real_t delta);
};

#endif