#ifndef A4_ACTION_H
#define A4_ACTION_H

#include "../common.h"

// inclusion model when using templates
// https://stackoverflow.com/a/3705774
template <class T>
class Action {
   protected:
    T *base;

   public:
    Action(T *base) { this->base = base; }

    void set_base(T *base) { this->base = base; }

    T *get_base() { return base; }

    virtual void init() = 0;
    virtual void tick(real_t delta) = 0;
};

#endif