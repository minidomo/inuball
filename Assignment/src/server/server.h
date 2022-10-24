#ifndef A2_SERVER_H
#define A2_SERVER_H

#include <NetworkedMultiplayerENet.hpp>
#include <Node.hpp>
#include <RandomNumberGenerator.hpp>
#include <SceneTree.hpp>

#include "./common.h"

class Server : public Node {
    GODOT_CLASS(Server, Node);

   private:
    RandomNumberGenerator* random;
    NetworkedMultiplayerENet* network;
    int64_t PORT;
    int64_t MAX_PLAYERS;

    Dictionary lobbies;
    Dictionary clients;

    String generate_random_team();
    int64_t generate_unique_lobby_id();
    int find_client_index(Array clients, int64_t client_id);
    void remove_client_from_lobby(int64_t client_id, int64_t lobby_id);
    void remove_null_lobbies();

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void start_server();
    void client_connected(int64_t id);
    void client_disconnected(int64_t id);

    // remote functions
    void request_register_client(int64_t client_id, String username);
    void request_all_lobbies(int64_t client_id);
    void request_create_lobby(int64_t client_id, Dictionary data);
    void request_join_lobby(int64_t client_id, int64_t lobby_id);
    void request_game_start(int64_t lobby_id);
    void request_update_username(int64_t client_id, String username);
    void request_change_team(int64_t client_id, int64_t lobby_id);
    void request_leave_lobby(int64_t client_id, int64_t lobby_id);
    void request_spawn_player(int64_t client_id, int64_t lobby_id);
    void request_update_player(int64_t client_id, int64_t lobby_id,
                               Vector3 translation, Vector3 rotation);
    void request_update_entity(int64_t client_id, int64_t lobby_id,
                               int64_t entity_id, Vector3 translation,
                               Vector3 rotation, Vector3 velocity);
    void request_game_end(int64_t lobby_id);
    void request_return_to_lobby(int64_t client_id, int64_t lobby_id);
    void request_ask_for_stats(int64_t client_id, int lobby_id);
    void request_send_stats_for(int64_t client_id, int lobby_id, uint64_t timer,
                                uint64_t red_chicken, uint64_t red_sheep,
                                uint64_t blue_chicken, uint64_t blue_sheep);
};

#endif