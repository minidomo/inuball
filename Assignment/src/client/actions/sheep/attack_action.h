#ifndef A4_ATTACK_ACTION_H
#define A4_ATTACK_ACTION_H

#include "../../common.h"
#include "../action.h"

class Sheep;

class AttackAction : public Action {
   private:
    Sheep *sheep;

   public:
    AttackAction(Sheep *sheep);

    virtual void init();
    virtual void tick(real_t delta);
};

#endif