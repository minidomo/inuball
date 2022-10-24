#include "./client.h"

#include <PackedScene.hpp>
#include <ResourceLoader.hpp>
#include <Timer.hpp>

#include "entity.h"

void Client::_register_methods() {
    register_method("_ready", &Client::_ready);

    register_method("connect_to_server", &Client::connect_to_server);
    register_method("connected_ok", &Client::connected_ok);
    register_method("connected_fail", &Client::connected_fail);
    register_method("server_disconnected", &Client::server_disconnected);
    register_method("game_ready", &Client::game_ready);
    register_method("spawn_current_players", &Client::spawn_current_players);

    register_method("response_all_lobbies", &Client::response_all_lobbies,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_signal<Client>("received_all_lobbies", "data",
                            GODOT_VARIANT_TYPE_DICTIONARY);

    register_method("response_create_lobby", &Client::response_create_lobby,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_signal<Client>("received_create_lobby", "lobby_id",
                            GODOT_VARIANT_TYPE_INT);

    register_method("response_join_lobby", &Client::response_join_lobby,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_method("response_join_game", &Client::response_join_game,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_method("response_game_start", &Client::response_game_start,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_method("response_spawn_player", &Client::response_spawn_player,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_method("response_update_lobby", &Client::response_update_lobby,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_method("respond_update_stats", &Client::respond_update_stats,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_method("response_get_stats_for", &Client::response_get_stats_for,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_signal<Client>("received_spawn_player", "info",
                            GODOT_VARIANT_TYPE_DICTIONARY, "self_id",
                            GODOT_VARIANT_TYPE_INT);

    register_signal<Client>("get_update_stats", "client_id",
                            GODOT_VARIANT_TYPE_INT, "lobby_id",
                            GODOT_VARIANT_TYPE_INT);

    register_signal<Client>("update_player_state", "client_id",
                            GODOT_VARIANT_TYPE_INT, "translation",
                            GODOT_VARIANT_TYPE_VECTOR3, "rotation",
                            GODOT_VARIANT_TYPE_VECTOR3);

    register_signal<Client>("update_stats", "timer", GODOT_VARIANT_TYPE_INT,
                            "red_chicken", GODOT_VARIANT_TYPE_INT, "red_sheep",
                            GODOT_VARIANT_TYPE_INT, "blue_chicken",
                            GODOT_VARIANT_TYPE_INT, "blue_sheep",
                            GODOT_VARIANT_TYPE_INT);

    register_method("response_leave_lobby", &Client::response_leave_lobby,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_signal<Client>("lobby_data_updated", Dictionary());

    register_method("response_update_player", &Client::response_update_player,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_method("response_update_entity", &Client::response_update_entity,
                    GODOT_METHOD_RPC_MODE_REMOTE);

    register_method("response_return_to_lobby",
                    &Client::response_return_to_lobby,
                    GODOT_METHOD_RPC_MODE_REMOTE);
}

void Client::_init() {
    network = NetworkedMultiplayerENet::_new();
    ip = "127.0.0.1";
    PORT = 3234;
    username = "Username";
}

Client* Client::get_singleton(Node* node) {
    // must match with the server's singleton which we also set to /root/Server
    return node->get_node<Client>("/root/Server");
}

void Client::_ready() {
    get_tree()->connect("server_disconnected", this, "server_disconnected");
    get_tree()->connect("connection_failed", this, "connected_fail");
    get_tree()->connect("connected_to_server", this, "connected_ok");
}

void Client::connect_to_server(String ip) {
    if (network->get_connection_status() != network->CONNECTION_DISCONNECTED) {
        network->close_connection();
    }

    this->ip = ip;
    Error error = network->create_client(ip, PORT);

    if (error == Error::OK) {
        get_tree()->set_network_peer(network);
    }
}

void Client::connected_ok() {
    client_id = get_tree()->get_network_unique_id();
    username = "user" + String::num_int64(client_id);
    rpc_id(1, "request_register_client", client_id, username);

    Godot::print("Successfully connected to server, client id: " +
                 String::num_int64(client_id));
}

void Client::connected_fail() { Godot::print("Failed to connect"); }

void Client::server_disconnected() { Godot::print("Server disconnected"); }

void Client::set_username(String username) { this->username = username; }

String Client::get_username() { return username; }

void Client::set_client_id(int64_t client_id) { this->client_id = client_id; }

int64_t Client::get_client_id() { return client_id; }

void Client::set_lobby_data(Dictionary lobby_data) {
    this->lobby_data = lobby_data;
}

Dictionary Client::get_lobby_data() { return lobby_data; }

void Client::request_all_lobbies() {
    rpc_id(1, "request_all_lobbies", client_id);
}

void Client::response_all_lobbies(Dictionary data) {
    emit_signal("received_all_lobbies", data);
}

void Client::request_create_lobby(Dictionary data) {
    rpc_id(1, "request_create_lobby", client_id, data);
}

void Client::response_create_lobby(int64_t lobby_id) {
    emit_signal("received_create_lobby", lobby_id);
}

void Client::request_join_lobby(int64_t lobby_id) {
    rpc_id(1, "request_join_lobby", client_id, lobby_id);
}

void Client::response_join_lobby(Dictionary data) {
    lobby_data = data;
    get_tree()->change_scene("res://scenes/gui/MultiplayerLobby.tscn");
}

void Client::response_join_game(Dictionary data, Dictionary game_data) {
    lobby_data = data;
    response_game_start();

    Timer* timer = Timer()._new();
    timer->set_one_shot(true);
    timer->set_wait_time(.3);
    timer->connect("timeout", this, "spawn_current_players");
    add_child(timer);
    timer->start();
}

void Client::request_update_username() {
    rpc_id(1, "request_update_username", client_id, username);
}

void Client::request_change_team() {
    rpc_id(1, "request_change_team", client_id, lobby_data["id"]);
}

void Client::response_update_lobby(Dictionary data) {
    lobby_data = data;
    emit_signal("lobby_data_updated");
}

void Client::request_leave_lobby() {
    rpc_id(1, "request_leave_lobby", client_id, lobby_data["id"]);
}

void Client::response_leave_lobby() {
    lobby_data = Dictionary();
    get_tree()->change_scene("res://scenes/gui/MultiplayerMainMenu.tscn");
}

void Client::request_game_start() {
    rpc_id(1, "request_game_start", lobby_data["id"]);
}

void Client::response_game_start() {
    get_tree()->change_scene(
        "res://scenes/playground/CustomWorldPlayground.tscn");
    // Would do this but viewport has a conflict on importing due to Camera
    // class. Don't want to rename it get_tree()->get_root()->connect("ready",
    // this, "game_ready");
}

void Client::game_ready() {
    response_spawn_player(this->client_id);
    request_spawn_player();
}

void Client::spawn_current_players() {
    // Spawn in other players
    Array clients = lobby_data["clients"];
    for (int i = 0; i < clients.size(); i++) {
        Dictionary client = clients[i];
        uint64_t client_id = client["id"];
        if (client_id != this->client_id) {
            Godot::print("Spawning...");
            Godot::print(String::num_int64(client_id));
            emit_signal("received_spawn_player", client, this->client_id);
        }
    }

    // Update stats as well
    request_ask_for_stats();
}

void Client::request_spawn_player() {
    rpc_id(1, "request_spawn_player", client_id, lobby_data["id"]);
}

void Client::respond_update_stats(uint64_t timer, uint64_t red_chicken,
                                  uint64_t red_sheep, uint64_t blue_chicken,
                                  uint64_t blue_sheep) {
    emit_signal("update_stats", timer, red_chicken, red_sheep, blue_chicken,
                blue_sheep);
}

void Client::request_ask_for_stats() {
    rpc_id(1, "request_ask_for_stats", client_id, lobby_data["id"]);
}

void Client::request_send_stats_for(uint64_t client_id, uint64_t lobby_id,
                                    uint64_t timer, uint64_t red_chicken,
                                    uint64_t red_sheep, uint64_t blue_chicken,
                                    uint64_t blue_sheep) {
    rpc_id(1, "request_send_stats_for", client_id, lobby_id, timer, red_chicken,
           red_sheep, blue_chicken, blue_sheep);
}

void Client::response_get_stats_for(uint64_t client_id, uint64_t lobby_id) {
    emit_signal("get_update_stats", client_id, lobby_id);
}

void Client::response_spawn_player(int64_t client_id) {
    Godot::print(String::num_int64(client_id));
    Array clients = lobby_data["clients"];
    for (int i = 0; i < clients.size(); i++) {
        Dictionary client = clients[i];
        int64_t cur_id = client["id"];
        if (cur_id == client_id) {
            emit_signal("received_spawn_player", client, this->client_id);
            break;
        }
    }
}

void Client::update_player(Vector3 translation, Vector3 rotation) {
    rpc_unreliable_id(1, "request_update_player", client_id, lobby_data["id"],
                      translation, rotation);
}

void Client::response_update_player(uint64_t client_id, Vector3 translation,
                                    Vector3 rotation) {
    emit_signal("update_player_state", client_id, translation, rotation);
}

void Client::response_update_entity(uint64_t entity_id, Vector3 translation,
                                    Vector3 rotation, Vector3 velocity) {
    String status = lobby_data["status"];

    if (status == "in progress" && Entity::entity_valid(entity_id)) {
        auto entity = Entity::get_entity(entity_id);
        entity->call_deferred("set_translation", translation);
        entity->call_deferred("set_rotation", rotation);
        entity->call_deferred("set_linear_velocity", velocity);
    }
}

void Client::update_entity(uint64_t entity, Vector3 translation,
                           Vector3 rotation, Vector3 velocity) {
    rpc_unreliable_id(1, "request_update_entity", client_id, lobby_data["id"],
                      entity, translation, rotation, velocity);
}

void Client::request_game_end() {
    rpc_id(1, "request_game_end", lobby_data["id"]);
}

void Client::request_return_to_lobby() {
    rpc_id(1, "request_return_to_lobby", client_id, lobby_data["id"]);
}

void Client::response_return_to_lobby(Dictionary data) {
    lobby_data = data;
    get_tree()->change_scene("res://scenes/gui/MultiplayerLobby.tscn");
}