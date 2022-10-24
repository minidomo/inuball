
#ifndef MARK_OF_THE_WOLVES_GLOBALS_H
#define MARK_OF_THE_WOLVES_GLOBALS_H

#include <Godot.hpp>
#include <Node.hpp>

struct GLOBALS {
    static GLOBALS instance;

    godot::Node *player = nullptr;
    godot::Node *goals[2] = {0};
    uint32_t world_size = 0;

    godot::Node *stats_container = nullptr;
    godot::Node *timer = nullptr;

    bool multiplayer = false;

   protected:
    GLOBALS() {}
};

#endif
