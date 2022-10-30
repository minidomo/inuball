#ifndef A4_FLEE_ACTION_H
#define A4_FLEE_ACTION_H

#include "../../common.h"
#include "../action.h"

class Sheep;

class FleeAction : public Action {
   private:
    Sheep *sheep;

   public:
    FleeAction(Sheep *sheep);

    virtual void init();
    virtual void tick(real_t delta);
};

#endif