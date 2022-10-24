

#ifndef KILL_FLOOR_H
#define KILL_FLOOR_H

#include <Area.hpp>
#include <RigidBody.hpp>

#include "../common.h"
#include "../globals.h"

class KillFloor : public Area {
    GODOT_CLASS(KillFloor, Area)

   public:
    void _init() { this->connect("body_entered", this, "body_entered"); }

    void body_entered(Node *other) {
        static bool init = false;
        if (!init) {
            init = true;
            return;
        }
        auto s = Object::cast_to<Spatial>(other);
        if (!s) return;
        auto size = (real_t)GLOBALS::instance.world_size;
        auto pos = s->get_translation();
        auto vel = Vector3(0.0f, 0.0f, 0.0f);
        if (pos.x < 0) {
            pos.x = 1.0f;
            vel.x = 1.0f;
        }
        if (pos.x > size) {
            pos.x = size - 1.0f;
            vel.x = -1.0f;
        }
        if (pos.z < 0) {
            pos.z = 1.0f;
            vel.z = 1.0f;
        }
        if (pos.z > size) {
            pos.z = size - 1.0f;
            vel.z = 1.0f;
        }
        pos.y = -pos.y + 1;
        s->set_translation(pos);
        auto r = Object::cast_to<RigidBody>(s);
        if (!r) return;
        r->set_linear_velocity(vel.normalized() * 5);
    }

    static void _register_methods() {
        register_method("body_entered", &KillFloor::body_entered);
    }
};

#endif
