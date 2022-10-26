
#ifndef ENTITY_H
#define ENTITY_H

#include <PhysicsBody.hpp>
#include <random>
#include <vector>

#include "../common.h"
#include "../globals.h"
#include "../singletons/client.h"
#include "player.h"

class Entity {
    static vector<godot::PhysicsBody *> entities;

   public:
    static uint64_t register_entity(PhysicsBody *entity) {
        entities.push_back(entity);
        return entities.size();
    }
    static godot::PhysicsBody *get_entity(uint64_t id) {
        return entities[id - 1];
    }

    static void clear_entities() { entities.clear(); }

    static bool entity_valid(uint64_t id) {
        return 0 <= id - 1 && id - 1 < entities.size();
    }

    static void entity_process(uint64_t entity_id) {
        auto entity = get_entity(entity_id);
        if (!entity) return;
        auto players = GLOBALS::instance.player->get_parent()->get_children();
        real_t min = 1e12;
        int64_t min_id = -1;
        for (int i = 0; i < players.size(); i++) {
            auto player = Object::cast_to<Player>(players[i]);
            if (!player) continue;
            auto m = (entity->get_translation() - player->get_translation())
                         .length_squared();
            if (m > min) continue;
            min = m;
            min_id = player->get_id();
        }
        auto master =
            min_id ==
            Object::cast_to<Player>(GLOBALS::instance.player)->get_id();
        entity->set_physics_process(master);
        if (master)
            Client::get_singleton(entity)->update_entity(
                entity_id, entity->get_translation(), entity->get_rotation(),
                Vector3{0, 0, 0});
    }
};

#endif
