
#ifndef GOAL_H
#define GOAL_H

#include <Area.hpp>
#include <Label.hpp>
#include <MeshInstance.hpp>
#include <RigidBody.hpp>
#include <SpatialMaterial.hpp>

#include "../common.h"
#include "../globals.h"
#include "entity.h"

enum TEAM_COLOR {
    RED = 0,
    BLUE = 1,
    NUM_COLORS = 2,
};

class Goal : public RigidBody {
    GODOT_CLASS(Goal, RigidBody)

   protected:
    TEAM_COLOR color;
    real_t time_since_update = 0;
    uint64_t entity_id;

   public:
    void _init() { this->entity_id = Entity::register_entity(this); }
    void _ready() {
        static uint32_t color = 0;
        this->color = (TEAM_COLOR)color++;
        if (color == NUM_COLORS) color = 0;

        auto mesh = Object::cast_to<MeshInstance>(this->get_node("./Mesh"));
        Ref<SpatialMaterial> mat = mesh->get_surface_material(0)->duplicate();
        Color emit;
        switch (this->color) {
            case RED:
                emit = Color(255, 0, 0, 255);
                break;
            case BLUE:
                emit = Color(0, 0, 255, 255);
                break;
            default:
                return;
        }
        mat->set_emission(emit);
        mesh->set_surface_material(0, mat);

        this->get_node("./Mesh/Area")
            ->connect("body_entered", this, "body_entered");
    }
    void _process(real_t t) { Entity::entity_process(this->entity_id); }
    void _physics_process(real_t t) {}

    void body_entered(Node *other) {
        if (other->has_method("entered_goal"))
            other->call("entered_goal", this);
    }

    static void _register_methods() {
        register_method("_ready", &Goal::_ready);
        register_method("_process", &Goal::_process);
        register_method("_physics_process", &Goal::_physics_process);
        register_method("body_entered", &Goal::body_entered);
    }

    void score(const char *object_name, uint32_t worth, uint32_t num) {
        auto stats = GLOBALS::instance.stats_container;
        char buffer[64];
        const char *teamcolor;
        switch (this->color) {
            case RED:
                teamcolor = "Red";
                break;
            case BLUE:
                teamcolor = "Blue";
                break;
            default:
                return;
        };
        snprintf(buffer, 64, "Score%s", teamcolor);
        auto score = Object::cast_to<Label>(stats->get_node(buffer));
        snprintf(buffer, 64, "%s%s", object_name, teamcolor);
        auto counter = Object::cast_to<Label>(stats->get_node(buffer));
        if (score)
            score->set_text(
                String::num_int64(score->get_text().to_int() + num * worth) +
                "\n");
        if (counter)
            counter->set_text(
                String::num_int64(counter->get_text().to_int() + num) + "\n");
    }

    int64_t get_score(const char *object_name, int64_t &counter) {
        auto stats = GLOBALS::instance.stats_container;
        char buffer[64];
        const char *teamcolor;
        switch (this->color) {
            case RED:
                teamcolor = "Red";
                break;
            case BLUE:
                teamcolor = "Blue";
                break;
            default:
                return -1;
        };
        snprintf(buffer, 64, "%s%s", object_name, teamcolor);
        auto counter_label = Object::cast_to<Label>(stats->get_node(buffer));
        counter = counter_label->get_text().to_int();
    }
};

#endif
