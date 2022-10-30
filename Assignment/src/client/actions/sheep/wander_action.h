#ifndef A4_WANDER_ACTION_H
#define A4_WANDER_ACTION_H

#include "../../common.h"
#include "../action.h"

class Sheep;

class WanderAction : public Action {
   private:
    Sheep *sheep;

   public:
    WanderAction(Sheep *sheep);

    virtual void init();
    virtual void tick(real_t delta);
};

#endif