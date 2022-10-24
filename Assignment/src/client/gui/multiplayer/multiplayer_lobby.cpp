#include "./multiplayer_lobby.h"

#include <PackedScene.hpp>
#include <Ref.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>

#include "../../debug.h"
#include "../../singletons/client.h"
#include "../../util.h"
#include "./multiplayer_lobby_player.h"

void MultiplayerLobby::_register_methods() {
    register_method("_ready", &MultiplayerLobby::_ready);

    register_method("on_start_button_pressed",
                    &MultiplayerLobby::on_start_button_pressed);
    register_property<MultiplayerLobby, NodePath>(
        "start_button_path", &MultiplayerLobby::start_button_path, NodePath());

    register_method("on_leave_button_pressed",
                    &MultiplayerLobby::on_leave_button_pressed);
    register_property<MultiplayerLobby, NodePath>(
        "leave_button_path", &MultiplayerLobby::leave_button_path, NodePath());

    register_method("on_change_team_button_pressed",
                    &MultiplayerLobby::on_change_team_button_pressed);
    register_property<MultiplayerLobby, NodePath>(
        "change_team_button_path", &MultiplayerLobby::change_team_button_path,
        NodePath());

    register_property<MultiplayerLobby, NodePath>(
        "title_label_path", &MultiplayerLobby::title_label_path, NodePath());

    register_property<MultiplayerLobby, NodePath>(
        "lobby_size_label_path", &MultiplayerLobby::lobby_size_label_path,
        NodePath());

    register_property<MultiplayerLobby, NodePath>(
        "game_length_label_path", &MultiplayerLobby::game_length_label_path,
        NodePath());

    register_property<MultiplayerLobby, NodePath>(
        "player_list_path", &MultiplayerLobby::player_list_path, NodePath());

    register_method("update_scene", &MultiplayerLobby::update_scene);
}

void MultiplayerLobby::_init() {
    start_button_path = NodePath();
    leave_button_path = NodePath();
    change_team_button_path = NodePath();
    title_label_path = NodePath();
    lobby_size_label_path = NodePath();
    game_length_label_path = NodePath();
    player_list_path = NodePath();
}

void MultiplayerLobby::_ready() {
    dev_assert(!start_button_path.is_empty());
    dev_assert(!leave_button_path.is_empty());
    dev_assert(!change_team_button_path.is_empty());
    dev_assert(!title_label_path.is_empty());
    dev_assert(!lobby_size_label_path.is_empty());
    dev_assert(!game_length_label_path.is_empty());
    dev_assert(!player_list_path.is_empty());

    start_button = Object::cast_to<BaseButton>(get_node(start_button_path));
    leave_button = Object::cast_to<BaseButton>(get_node(leave_button_path));
    change_team_button =
        Object::cast_to<BaseButton>(get_node(change_team_button_path));
    title_label = Object::cast_to<Label>(get_node(title_label_path));
    lobby_size_label = Object::cast_to<Label>(get_node(lobby_size_label_path));
    game_length_label =
        Object::cast_to<Label>(get_node(game_length_label_path));
    player_list = Object::cast_to<VBoxContainer>(get_node(player_list_path));

    dev_assert(start_button != nullptr);
    dev_assert(leave_button != nullptr);
    dev_assert(change_team_button != nullptr);
    dev_assert(title_label != nullptr);
    dev_assert(lobby_size_label != nullptr);
    dev_assert(game_length_label != nullptr);
    dev_assert(player_list != nullptr);

    start_button->connect("pressed", this, "on_start_button_pressed");
    leave_button->connect("pressed", this, "on_leave_button_pressed");
    change_team_button->connect("pressed", this,
                                "on_change_team_button_pressed");

    Client::get_singleton(this)->connect("lobby_data_updated", this,
                                         "update_scene");

    update_scene();
}

void MultiplayerLobby::on_start_button_pressed() {
    Client::get_singleton(this)->request_game_start();
}

void MultiplayerLobby::on_leave_button_pressed() {
    Client::get_singleton(this)->request_leave_lobby();
}

void MultiplayerLobby::on_change_team_button_pressed() {
    Client::get_singleton(this)->request_change_team();
}

void MultiplayerLobby::update_scene() {
    Dictionary data = Client::get_singleton(this)->get_lobby_data();

    title_label->set_text(data["title"]);

    String game_length_label_text =
        "[" + Util::get_time_str(data["game_length"]) + "]";
    game_length_label->set_text(game_length_label_text);

    Array clients = data["clients"];

    String lobby_size_text = "(" + String::num_int64(clients.size()) + "/" +
                             String::num_int64(data["max_size"]) + ")";
    lobby_size_label->set_text(lobby_size_text);

    Util::delete_children(player_list);

    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(
        "res://scenes/gui/MultiplayerLobbyPlayer.tscn");

    for (int i = 0; i < clients.size(); i++) {
        Dictionary client = clients[i];
        Godot::print(client.to_json());

        MultiplayerLobbyPlayer *player =
            Object::cast_to<MultiplayerLobbyPlayer>(scene->instance());

        dev_assert(player != nullptr);

        player->set_team(client["team"]);
        player->set_username(client["username"]);

        bool in_lobby = client["in_lobby"];
        String in_game_status = in_lobby ? "" : "in game";
        player->set_in_game_status(in_game_status);

        player_list->add_child(player);
    }
}