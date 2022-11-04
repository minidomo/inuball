
#ifndef ANIMAL_H
#define ANIMAL_H

#include <AudioStreamPlayer.hpp>
#include <GlobalConstants.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <RigidBody.hpp>

#include "../../globals.h"
#include "../goal.h"
#include "../player.h"
#include "../signals/looking_at_receiver.h"
#include "entity.h"

using namespace godot;

typedef enum {
    NONE,
    CHICKEN,
    SHEEP,
    NUM_ANIMALS,
} Animals;

#define thisnode this->get_child(0)->get_parent()

template <size_t id_t>
class Animal : public KinematicBody,
               public LookingAtReceiver,
               public iGameObject {
    GODOT_CLASS(Animal, KinematicBody)
   protected:
    bool inFocus;
    AudioStreamPlayer *interactStream;
    AudioStreamPlayer *deathStream;

    bool sfxOn = true;
    float sfxDb = 0.0;
    float sfxSens = 1.0;

    real_t time_since_update = 0;
    uint64_t entity_id;

    Vector3 velocity{0, 0, 0};
    float gravity;

   public:
    const size_t id = id_t;

    static void _register_methods();

    void _ready();
    void _init() {
        this->entity_id = Entity::register_entity(this);
        this->gravity = 50.0f;
    }

    virtual void collide_with_player(Player *player) {
        auto pv = player->velocity;
        auto vel = this->velocity;

        player->need_to_handle_collision = true;

        pv.y += pv.length();
        this->velocity = pv;

        player->velocity = vel;
    }

    void handleLookAt(Node *player, Node *target, Vector3 point, Vector3 normal,
                      real_t distance) override {
        if (thisnode == parent<0>(target))
            Object::cast_to<Player>(player)->set_selected(this);
    }

    static void attemptbreed(Animal *animal) {
        static uint8_t flipflop = 0;
        static Animal *parents[2] = {nullptr, nullptr};
        parents[flipflop++] = animal;
        flipflop %= 2;
        if (!parents[0] || !parents[1]) return;
        if (parents[0]->id != parents[1]->id) return;
        if (parents[0] == parents[1]) return;
        auto n = animal->duplicate();
        animal->get_parent()->add_child(n);
        parents[0] = parents[1] = nullptr;
    }

    void _process(real_t t) { Entity::entity_process(this->entity_id); }
    void _physics_process(real_t t) {
        if (!is_on_floor()) {
            velocity.y -= gravity * t;
        }

        velocity =
            move_and_slide_with_snap(velocity, Vector3::DOWN, Vector3::UP);

        Vector3 orientation = velocity;
        orientation.y = 0;
        orientation.normalize();
        orientation.rotate(Vector3::UP, Math::deg2rad(90.0f));

        if (orientation != Vector3::ZERO) {
            Vector3 origin = get_global_transform().origin;

            // the difference between target and origin needs to be large enough
            // so look_at works
            float multiplier = 50;
            Vector3 target = origin + orientation * multiplier;

            look_at(target, Vector3::UP);
        }
    }

    virtual bool interact_secondary(Node *player) override {
        attemptbreed(this);
        return true;
    }
    virtual bool interact_primary(Node *player) override {
        // Object::cast_to<Player>(player)->set_selected(nullptr);
        return false;
    }

    virtual void entered_goal(Goal *goal) {
        this->set_translation(Vector3(
            rand() / (float)RAND_MAX * GLOBALS::instance.world_size, 0.0f,
            rand() / (float)RAND_MAX * GLOBALS::instance.world_size));
    }
};

#include "chicken.h"
#include "sheep.h"

#endif
