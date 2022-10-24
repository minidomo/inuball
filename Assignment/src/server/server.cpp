#include "./server.h"

#include <IP.hpp>
#include <Label.hpp>
#include <VBoxContainer.hpp>

#include "./debug.h"

void Server::_register_methods() {
    register_method("_ready", &Server::_ready);
    register_method("start_server", &Server::start_server);
    register_method("client_connected", &Server::client_connected);
    register_method("client_disconnected", &Server::client_disconnected);

    register_method("request_register_client", &Server::request_register_client,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_all_lobbies", &Server::request_all_lobbies,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_create_lobby", &Server::request_create_lobby,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_join_lobby", &Server::request_join_lobby,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_game_start", &Server::request_game_start,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_update_username", &Server::request_update_username,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_change_team", &Server::request_change_team,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_leave_lobby", &Server::request_leave_lobby,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_spawn_player", &Server::request_spawn_player,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_game_start", &Server::request_game_start,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_update_player", &Server::request_update_player,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_update_entity", &Server::request_update_entity,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_game_end", &Server::request_game_end,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_return_to_lobby", &Server::request_return_to_lobby,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_ask_for_stats", &Server::request_ask_for_stats,
                    GODOT_METHOD_RPC_MODE_REMOTE);
    register_method("request_send_stats_for", &Server::request_send_stats_for,
                    GODOT_METHOD_RPC_MODE_REMOTE);
}

void Server::_init() {
    random = RandomNumberGenerator::_new();
    random->randomize();

    network = NetworkedMultiplayerENet::_new();
    PORT = 3234;
    MAX_PLAYERS = 24;
}

void Server::_ready() { start_server(); }

void Server::start_server() {
    network->create_server(PORT, MAX_PLAYERS);
    get_tree()->set_network_peer(network);

    network->connect("peer_connected", this, "client_connected");
    network->connect("peer_disconnected", this, "client_disconnected");

    auto ip_text_f = Object::cast_to<Label>(
        this->get_node("/root/Control/ScrollContainer/VBoxContainer/ip"));
    auto vbox = ip_text_f->get_parent();
    vbox->remove_child(ip_text_f);
    auto interfaces = IP::get_singleton()->get_local_interfaces();
    for (int j = 0; j < interfaces.size(); j++) {
        Dictionary interface = interfaces[j];
        String name = interface["friendly"];
        if (name.length() == 0) name = interface["name"];
        Array addresses = interface["addresses"];
        auto ip_text = Object::cast_to<Label>(ip_text_f->duplicate());
        name = "interface \"" + name + "\": ";
        for (int i = 0; i < addresses.size(); i++) {
            String ip = addresses[i];
            if (i) name += ", ";
            name += ip;
        }
        ip_text->set_text(name + "\n");
        vbox->add_child(ip_text);
    }
    ip_text_f->queue_free();

    Godot::print("Server started at 127.0.0.1:" + String::num_int64(PORT));
}

void Server::client_connected(int64_t client_id) {
    Godot::print("Client " + String::num_int64(client_id) + " Connected");
}

void Server::client_disconnected(int64_t client_id) {
    Godot::print("Client " + String::num_int64(client_id) + " Disconnected");

    String client_key = String::num_int64(client_id);
    Dictionary client = clients[client_key];

    int64_t lobby_id = client["lobby_id"];
    if (lobby_id != -1) {
        remove_client_from_lobby(client_id, lobby_id);
    }

    clients.erase(client_key);
}

void Server::request_register_client(int64_t client_id, String username) {
    Dictionary client;

    client["id"] = client_id;
    client["username"] = username;
    client["lobby_id"] = -1;

    clients[String::num_int64(client_id)] = client;
}

void Server::request_all_lobbies(int64_t client_id) {
    // somehow some null lobbies get into the lobbies data struct so remove them
    remove_null_lobbies();

    Array all_lobbies = lobbies.values();

    // ideally use an array but doesnt work well with rpc calls and signals, so
    // use a dictionary
    Dictionary ret;

    for (int i = 0; i < all_lobbies.size(); i++) {
        Dictionary lobby = all_lobbies[i];

        Dictionary lobby_card;
        lobby_card["id"] = lobby["id"];
        lobby_card["title"] = lobby["title"];
        lobby_card["status"] = lobby["status"];
        lobby_card["max_size"] = lobby["max_size"];
        lobby_card["game_length"] = lobby["game_length"];

        Array lobby_clients = lobby["clients"];
        lobby_card["current_size"] = lobby_clients.size();

        ret[String::num_int64(i)] = lobby_card;
    }

    rpc_id(client_id, "response_all_lobbies", ret);
}

void Server::request_create_lobby(int64_t client_id, Dictionary data) {
    Dictionary lobby;

    int64_t lobby_id = generate_unique_lobby_id();
    lobby["id"] = lobby_id;
    lobby["title"] = data["title"];
    lobby["max_size"] = data["max_size"];
    lobby["game_length"] = data["game_length"];
    lobby["status"] = "open";
    lobby["clients"] = Array();

    lobbies[String::num_int64(lobby_id)] = lobby;

    rpc_id(client_id, "response_create_lobby", lobby_id);
}

void Server::request_join_lobby(int64_t client_id, int64_t lobby_id) {
    String lobby_key = String::num_int64(lobby_id);

    // check if the lobby id exists. since the lobbies shown do not update
    // automatically when lobbies are made or destroyed, we need to check this
    if (!lobbies.has(lobby_key)) return;

    Dictionary lobby = lobbies[lobby_key];

    Array lobby_clients = lobby["clients"];
    int max_size = lobby["max_size"];

    if (lobby_clients.size() < max_size) {
        Dictionary client = clients[String::num_int64(client_id)];

        Dictionary lobby_client;
        lobby_client["id"] = client_id;
        lobby_client["username"] = client["username"];
        lobby_client["team"] = generate_random_team();
        lobby_client["in_lobby"] = true;

        lobby_clients.append(lobby_client);

        client["lobby_id"] = lobby_id;
        String status = lobby["status"];

        if (status == "open") {
            lobby_client["in_lobby"] = true;
            rpc_id(client_id, "response_join_lobby", lobby);
        } else if (status == "in progress") {
            lobby_client["in_lobby"] = false;
            Dictionary game_data;
            rpc_id(client_id, "response_join_game", lobby, game_data);
        }

        // update ui for existing clients
        for (int i = 0; i < lobby_clients.size() - 1; i++) {
            Dictionary cur_lobby_client = lobby_clients[i];
            rpc_id(cur_lobby_client["id"], "response_update_lobby", lobby);
        }
    }
}

void Server::request_game_start(int64_t lobby_id) {
    Dictionary lobby = lobbies[String::num_int64(lobby_id)];

    Array lobby_clients = lobby["clients"];

    // check to see that everyone is in the lobby
    for (int i = 0; i < lobby_clients.size(); i++) {
        Dictionary lobby_client = lobby_clients[i];
        bool in_lobby = lobby_client["in_lobby"];

        if (!in_lobby) return;
    }

    lobby["status"] = "in progress";

    for (int i = 0; i < lobby_clients.size(); i++) {
        Dictionary cur_client = lobby_clients[i];
        cur_client["in_lobby"] = false;
        rpc_id(cur_client["id"], "response_game_start");
    }
}

void Server::request_spawn_player(int64_t client_id, int64_t lobby_id) {
    Dictionary lobby = lobbies[String::num_int64(lobby_id)];
    Array lobby_clients = lobby["clients"];
    Godot::print("spawning");
    for (int i = 0; i < lobby_clients.size(); i++) {
        Dictionary cur_client = lobby_clients[i];
        // Client spawned their player before playing so
        // don't ask them to do it again
        uint64_t cur_client_id = cur_client["id"];
        if (cur_client_id != client_id) {
            rpc_id(cur_client_id, "response_spawn_player", client_id);
        }
    }
}

void Server::request_update_player(int64_t client_id, int64_t lobby_id,
                                   Vector3 translation, Vector3 rotation) {
    Dictionary lobby = lobbies[String::num_int64(lobby_id)];
    Array lobby_clients = lobby["clients"];
    for (int i = 0; i < lobby_clients.size(); i++) {
        Dictionary client = lobby_clients[i];
        uint64_t cur_client_id = client["id"];
        if (client_id != cur_client_id) {
            rpc_unreliable_id(cur_client_id, "response_update_player",
                              client_id, translation, rotation);
        }
    }
}

void Server::request_update_entity(int64_t client_id, int64_t lobby_id,
                                   int64_t entity_id, Vector3 translation,
                                   Vector3 rotation, Vector3 velocity) {
    Dictionary lobby = lobbies[String::num_int64(lobby_id)];
    Array lobby_clients = lobby["clients"];
    for (int i = 0; i < lobby_clients.size(); i++) {
        Dictionary client = lobby_clients[i];
        uint64_t cur_client_id = client["id"];
        if (client_id != cur_client_id) {
            rpc_unreliable_id(cur_client_id, "response_update_entity",
                              entity_id, translation, rotation, velocity);
        }
    }
}

void Server::request_ask_for_stats(int64_t client_id, int lobby_id) {
    Dictionary lobby = lobbies[String::num_int64(lobby_id)];
    Array lobby_clients = lobby["clients"];
    Dictionary client = lobby_clients[0];
    uint64_t cur_client_id = client["id"];
    rpc_id(cur_client_id, "response_get_stats_for", client_id, lobby_id);
}

void Server::request_send_stats_for(int64_t client_id, int lobby_id,
                                    uint64_t timer, uint64_t red_chicken,
                                    uint64_t red_sheep, uint64_t blue_chicken,
                                    uint64_t blue_sheep) {
    rpc_id(client_id, "respond_update_stats", timer, red_chicken, red_sheep,
           blue_chicken, blue_sheep);
}

String Server::generate_random_team() {
    bool is_red = random->randi() % 2;
    return is_red ? "red" : "blue";
}

int64_t Server::generate_unique_lobby_id() {
    int64_t id = -1;
    do {
        id = random->randi_range(0, 100000);
    } while (lobbies.has(id));
    return id;
}

void Server::request_update_username(int64_t client_id, String username) {
    Dictionary client = clients[String::num_int64(client_id)];
    client["username"] = username;
}

void Server::request_change_team(int64_t client_id, int64_t lobby_id) {
    String lobby_key = String::num_int64(lobby_id);

    dev_assert(lobbies.has(lobby_key));

    // find client in the lobby
    Dictionary lobby = lobbies[lobby_key];
    Array lobby_clients = lobby["clients"];
    int index = find_client_index(lobby_clients, client_id);

    dev_assert(index != -1);

    Dictionary client = lobby_clients[index];
    String team = client["team"];

    // change teams
    if (team == "red") {
        client["team"] = "blue";
    } else {
        client["team"] = "red";
    }

    // update ui for all clients in the lobby
    for (int i = 0; i < lobby_clients.size(); i++) {
        Dictionary lobby_client = lobby_clients[i];
        rpc_id(lobby_client["id"], "response_update_lobby", lobby);
    }
}

int Server::find_client_index(Array clients, int64_t client_id) {
    for (int i = 0; i < clients.size(); i++) {
        Dictionary client = clients[i];
        int64_t id = client["id"];

        if (id == client_id) {
            return i;
        }
    }

    return -1;
}

void Server::request_leave_lobby(int64_t client_id, int64_t lobby_id) {
    remove_client_from_lobby(client_id, lobby_id);
    rpc_id(client_id, "response_leave_lobby");
}

void Server::remove_client_from_lobby(int64_t client_id, int64_t lobby_id) {
    String lobby_key = String::num_int64(lobby_id);

    dev_assert(lobbies.has(lobby_key));

    Dictionary lobby = lobbies[lobby_key];
    Array lobby_clients = lobby["clients"];

    // find the specified client id in the lobby and remove it
    int index = find_client_index(lobby_clients, client_id);

    dev_assert(index != -1);

    lobby_clients.remove(index);

    if (lobby_clients.empty()) {
        // remove empty lobbies
        lobbies.erase(String::num_int64(lobby_id));
    } else {
        // update remaining clients in the lobby
        for (int i = 0; i < lobby_clients.size(); i++) {
            Dictionary lobby_client = lobby_clients[i];
            rpc_id(lobby_client["id"], "response_update_lobby", lobby);
        }
    }

    // reset client lobby id
    Dictionary client = clients[String::num_int64(client_id)];
    client["lobby_id"] = -1;
}

void Server::remove_null_lobbies() {
    Array keys = lobbies.keys();

    int removed = 0;

    for (int i = 0; i < keys.size(); i++) {
        String key = keys[i];
        Variant value = lobbies[key];

        if (value.get_type() == Variant::Type::NIL) {
            lobbies.erase(key);
            removed++;
        }
    }

    if (removed > 0) {
        Godot::print("found " + String::num_int64(removed) + " null lobbies");
    }
}

void Server::request_game_end(int64_t lobby_id) {
    Dictionary lobby = lobbies[String::num_int64(lobby_id)];
    lobby["status"] = "open";
}

void Server::request_return_to_lobby(int64_t client_id, int64_t lobby_id) {
    Dictionary lobby = lobbies[String::num_int64(lobby_id)];

    Array lobby_clients = lobby["clients"];
    int index = find_client_index(lobby_clients, client_id);

    // update client in_lobby status
    Dictionary cur_lobby_client = lobby_clients[index];
    cur_lobby_client["in_lobby"] = true;

    // the user will return to lobby so update other clients' ui
    for (int i = 0; i < lobby_clients.size(); i++) {
        Dictionary lobby_client = lobby_clients[i];
        int64_t c_id = lobby_client["id"];
        bool in_lobby = lobby_client["in_lobby"];

        if (c_id != client_id && in_lobby) {
            rpc_id(c_id, "response_update_lobby", lobby);
        }
    }

    // tell client to go back to lobby now
    rpc_id(client_id, "response_return_to_lobby", lobby);
}