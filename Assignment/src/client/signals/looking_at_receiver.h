
#ifndef LOOKING_AT_RECEIVER
#define LOOKING_AT_RECEIVER

#include <Godot.hpp>
#include <Node.hpp>
#include <Vector3.hpp>
#include "../globals.h"
#include "../common.h"

template <uint8_t num>
inline Node *parent(Node *n) {
    for(int i = 0; i < num; i++) {
        if(!n) return nullptr;
        n = n->get_parent();
    }
    return n;
}

class LookingAtReceiver {
public:

    virtual void handleLookAt(Node *player, Node *target, Vector3 point, Vector3 normal, real_t distance) = 0;

    static void subscribe(Node *node) {
        GLOBALS::instance.player->connect("looking_at", node, "handleLookAt");
    }
};

#endif
