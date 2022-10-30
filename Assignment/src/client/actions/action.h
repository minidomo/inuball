#ifndef A4_ACTION_H
#define A4_ACTION_H

#include "../common.h"

class Action {
   public:
    virtual void init() = 0;
    virtual void tick(real_t delta) = 0;
};

#endif