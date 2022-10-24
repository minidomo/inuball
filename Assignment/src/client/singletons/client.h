#ifndef A2_CLIENT_H
#define A2_CLIENT_H

#include <NetworkedMultiplayerENet.hpp>
#include <Node.hpp>
#include <SceneTree.hpp>

#include "./common.h"

class Client : public Node {
    GODOT_CLASS(Client, Node);

   private:
    String ip;
    int64_t PORT;

    NetworkedMultiplayerENet* network;

    String username;
    int64_t client_id;

    Dictionary lobby_data;

   public:
    static void _register_methods();
    static Client* get_singleton(Node* node);
    void _init();

    void _ready();
    void connect_to_server(String ip);
    void connected_ok();
    void connected_fail();
    void server_disconnected();

    void set_username(String username);
    String get_username();

    void set_client_id(int64_t client_id);
    int64_t get_client_id();

    void set_lobby_data(Dictionary lobby_data);
    Dictionary get_lobby_data();

    void game_ready();
    void spawn_current_players();
    void update_player(Vector3 translation, Vector3 rotation);
    void update_entity(uint64_t entity, Vector3 translation, Vector3 rotation,
                       Vector3 velocity);

    void request_all_lobbies();
    void request_create_lobby(Dictionary data);
    void request_join_lobby(int64_t lobby_id);
    void request_update_username();
    void request_change_team();
    void request_leave_lobby();
    void request_game_start();
    void request_spawn_player();
    void request_game_end();
    void request_return_to_lobby();

    // remote functions
    void response_all_lobbies(Dictionary data);
    void response_create_lobby(int64_t lobby_id);
    void response_join_lobby(Dictionary data);
    void response_join_game(Dictionary data, Dictionary game_data);
    void response_game_start();
    void response_update_lobby(Dictionary data);
    void response_leave_lobby();
    void response_spawn_player(int64_t client_id);
    void response_update_player(uint64_t client_id, Vector3 translation,
                                Vector3 rotation);
    void response_update_entity(uint64_t entity_id, Vector3 translation,
                                Vector3 rotation, Vector3 velocity);
    void response_return_to_lobby(Dictionary data);
    void respond_update_stats(uint64_t timer, uint64_t red_chicken,
                              uint64_t red_sheep, uint64_t blue_chicken,
                              uint64_t blue_sheep);
    void request_ask_for_stats();
    void request_send_stats_for(uint64_t client_id, uint64_t lobby_id,
                                uint64_t timer, uint64_t red_chicken,
                                uint64_t red_sheep, uint64_t blue_chicken,
                                uint64_t blue_sheep);
    void response_get_stats_for(uint64_t client_id, uint64_t lobby_id);
};

#endif