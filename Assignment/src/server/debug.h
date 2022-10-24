#ifndef A2_DEBUG_H
#define A2_DEBUG_H

#include <Godot.hpp>

// my intellisense doesn't see the already defined macro, __FUNCTION__, even
// though it's defined at runtime, so do this as a fix
#ifndef __FUNCTION__
#define __FUNCTION__ ""
#endif

#define dev_assert(condition)                                                  \
    if (!(condition)) {                                                        \
        godot::Godot::print_error("\"" #condition "\" is false", __FUNCTION__, \
                                  __FILE__, __LINE__);                         \
    }

#endif