
#include "custom_world.h"

#include <PackedScene.hpp>
#include <ResourceLoader.hpp>
#include <SpatialMaterial.hpp>
#include <ctime>
#include <thread>

#include "../debug.h"
#include "../entity.h"
#include "../gui/timer.h"
#include "../objects/animals/chicken.h"
#include "../objects/goal.h"
#include "../objects/player.h"
#include "../singletons/client.h"

void CustomWorld::_register_methods() {
    register_method("_process", &CustomWorld::_process);
    register_method("_ready", &CustomWorld::_ready);
    register_method("spawn_player", &CustomWorld::spawn_player);
    register_method("_update_player_state", &CustomWorld::_update_player_state);
    register_method("_update_stats", &CustomWorld::_update_stats);
    register_method("_get_update_stats", &CustomWorld::_get_update_stats);
    register_property<CustomWorld, NodePath>(
        "Player Path", &CustomWorld::player_path, NodePath());
    register_property<CustomWorld, NodePath>(
        "Players Path", &CustomWorld::players_path, NodePath());
}

CustomWorld::CustomWorld() {
    srand((uint32_t)time(nullptr));
    this->noise->set_seed(0x1234);
}

CustomWorld::~CustomWorld() {}

void CustomWorld::_init() {
    player_path = NodePath();
    players_path = NodePath();

    Entity::clear_entities();
}

#define spawn_animal(animal, num)                                            \
    {                                                                        \
        Ref<PackedScene> animal_f = ResourceLoader::get_singleton()->load(   \
            "res://scenes/objects/animals/" animal ".tscn");                 \
        for (int i = 0; i < num; i++) {                                      \
            auto n = Object::cast_to<Spatial>(animal_f->instance());         \
            auto x = rand() / (float)RAND_MAX, z = rand() / (float)RAND_MAX; \
            n->set_translation(                                              \
                Vector3(x* Chunk::size, Chunk::height, z* Chunk::size));     \
            this->add_child(n);                                              \
        }                                                                    \
    }

void CustomWorld::_ready() {
    // create the enclosure so player can't fall off world
    auto mesh_instance =
        Object::cast_to<MeshInstance>(this->get_node("./bounds"));
    mesh_instance->set_translation(
        Vector3(Chunk::size / 2.0f, -Chunk::height - 2.0f, Chunk::size / 2.0f));
    mesh_instance->set_scale(
        Vector3(Chunk::size / 2.0f, Chunk::height * 10.0f, Chunk::size / 2.0f));
    mesh_instance->set_visible(false);
    mesh_instance->create_trimesh_collision();

    // add a kill floor -- objects that enter this area have fallen through the
    // world
    auto floor = Object::cast_to<Spatial>(this->get_node("./kill_floor"));
    floor->set_scale(Vector3((real_t)Chunk::size, 1.0f, (real_t)Chunk::size));
    floor->set_translation(Vector3(0.0f, -Chunk::height - 1.0f, 0.0f));
    GLOBALS::instance.world_size = Chunk::size;

    // players init
    dev_assert(!players_path.is_empty());
    players = Object::cast_to<Node>(get_node(players_path));

    Ref<PackedScene> playerRef = ResourceLoader::get_singleton()->load(
        "res://scenes/objects/Player.tscn");
    Spatial* player = Object::cast_to<Spatial>(playerRef->instance());
    players->add_child(player);
    this->player = player;
    this->player->set_translation(
        Vector3(Chunk::size / 2, Chunk::height, Chunk::size / 2));
    GLOBALS::instance.player = this->player;
    Client::get_singleton(this)->connect("received_spawn_player", this,
                                         "spawn_player");
    Client::get_singleton(this)->connect("update_player_state", this,
                                         "_update_player_state");
    Client::get_singleton(this)->connect("update_stats", this, "_update_stats");
    Client::get_singleton(this)->connect("get_update_stats", this,
                                         "_get_update_stats");

    // create a single chunk to be the field
    current = Chunk::_new();
    current->create(0, 0, this->noise);
    this->add_child(current);

    // set global stats object
    GLOBALS::instance.stats_container = this->get_parent()->get_node(
        "Stats/VBoxContainer/VSplitContainer/GridContainer");

    // create goals
    Ref<PackedScene> goal_f =
        ResourceLoader::get_singleton()->load("res://scenes/objects/Goal.tscn");
    auto goal1 = Object::cast_to<Spatial>(goal_f->instance());
    auto goal2 = Object::cast_to<Spatial>(goal_f->instance());
    goal1->set_translation(
        Vector3(Chunk::size / 2.0f, Chunk::height, Chunk::size / 4.0f));
    goal2->set_translation(
        Vector3(Chunk::size / 2.0f, Chunk::height, Chunk::size * 3.0f / 4.0f));
    this->add_child(goal1);
    this->add_child(goal2);
    GLOBALS::instance.goals[0] = goal1;
    GLOBALS::instance.goals[1] = goal2;

    // populate with animals
    spawn_animal("Chicken", 10);
    Array children = this->get_children();
    chickens = Array();
    for (int i = 0; i < children.size(); i++) {
        Chicken* tmp = Object::cast_to<Chicken>(children[i]);
        if (tmp) {
            chickens.push_back(tmp);
            // tmp->set_translation(
            //     Vector3(Chunk::size / 2 + chickens.size() % 5 * 2, 5,
            //             Chunk::size / 2 + chickens.size() / 5 * 2));
        }
    }
    for (int i = 0; i < chickens.size(); i++) {
        Chicken* tmp = Object::cast_to<Chicken>(chickens[i]);
        tmp->set_chickens(chickens);
    }

    spawn_animal("Sheep", 6);
    Client::get_singleton(this)->game_ready();
}

void CustomWorld::spawn_player(Dictionary info, uint32_t self_id) {
    uint32_t player_id = info["id"];
    String name = info["username"];
    Player* player;
    if (player_id == self_id) {
        player = Object::cast_to<Player>(this->player);
    } else {
        Ref<PackedScene> playerRef = ResourceLoader::get_singleton()->load(
            "res://scenes/objects/Player.tscn");
        player = Object::cast_to<Player>(playerRef->instance());
        player->set_controllable(false);
        uint64_t client_id = info["id"];
        player->set_translation(
            Vector3(Chunk::size / 2, Chunk::height, Chunk::size / 2));
        players->add_child(player);
    }
    player->set_id(player_id);
    player->set_username(name);
}

void CustomWorld::_update_player_state(uint64_t client_id, Vector3 translation,
                                       Vector3 rotation) {
    Array player_list = players->get_children();
    for (int i = 0; i < player_list.size(); i++) {
        Player* player = Object::cast_to<Player>(player_list[i]);
        if (player->get_id() != client_id) continue;
        player->set_translation(translation);
        player->set_rotation(rotation);
        player->touch();
        return;
    }
}

void CustomWorld::_update_stats(uint64_t timer, uint64_t red_chicken,
                                uint64_t red_sheep, uint64_t blue_chicken,
                                uint64_t blue_sheep) {
    Object::cast_to<GUITimer>(GLOBALS::instance.timer)->set_time(timer);
    auto goal1 = Object::cast_to<Goal>(GLOBALS::instance.goals[0]);
    auto goal2 = Object::cast_to<Goal>(GLOBALS::instance.goals[1]);
    goal1->score("Chicken", 5, red_chicken);
    goal1->score("Sheep", 1, red_sheep);
    goal2->score("Chicken", 5, blue_chicken);
    goal2->score("Sheep", 1, blue_sheep);
}

void CustomWorld::_get_update_stats(uint64_t client_id, uint64_t lobby_id) {
    int64_t red_chicken = 0;
    int64_t red_sheep = 0;
    int64_t blue_chicken = 0;
    int64_t blue_sheep = 0;
    auto goal1 = Object::cast_to<Goal>(GLOBALS::instance.goals[0]);
    auto goal2 = Object::cast_to<Goal>(GLOBALS::instance.goals[1]);
    goal1->get_score("Chicken", red_chicken);
    goal1->get_score("Sheep", red_sheep);
    goal2->get_score("Chicken", blue_chicken);
    goal2->get_score("Sheep", blue_sheep);
    Godot::print("Getting");
    Godot::print(String::num_int64(red_chicken));
    Godot::print(String::num_int64(red_sheep));
    Godot::print(String::num_int64(blue_chicken));
    Godot::print(String::num_int64(blue_sheep));
    Client::get_singleton(this)->request_send_stats_for(
        client_id, lobby_id,
        Object::cast_to<GUITimer>(GLOBALS::instance.timer)->get_time(),
        red_chicken, red_sheep, blue_chicken, blue_sheep);
}

void CustomWorld::_process(float delta) { time_passed += delta; }
