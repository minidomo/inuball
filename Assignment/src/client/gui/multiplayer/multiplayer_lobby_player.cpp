#include "./multiplayer_lobby_player.h"

#include "../../debug.h"

void MultiplayerLobbyPlayer::_register_methods() {
    register_method("_ready", &MultiplayerLobbyPlayer::_ready);

    register_property<MultiplayerLobbyPlayer, NodePath>(
        "team_label_path", &MultiplayerLobbyPlayer::team_label_path,
        NodePath());

    register_property<MultiplayerLobbyPlayer, NodePath>(
        "username_label_path", &MultiplayerLobbyPlayer::username_label_path,
        NodePath());

    register_property<MultiplayerLobbyPlayer, NodePath>(
        "in_game_status_label_path",
        &MultiplayerLobbyPlayer::in_game_status_label_path, NodePath());
}

void MultiplayerLobbyPlayer::_init() {
    team_label_path = NodePath();
    username_label_path = NodePath();
    in_game_status_label_path = NodePath();
}

void MultiplayerLobbyPlayer::_ready() {
    dev_assert(!team_label_path.is_empty());
    dev_assert(!username_label_path.is_empty());
    dev_assert(!in_game_status_label_path.is_empty());

    team_label = Object::cast_to<Label>(get_node(team_label_path));
    username_label = Object::cast_to<Label>(get_node(username_label_path));
    in_game_status_label =
        Object::cast_to<Label>(get_node(in_game_status_label_path));

    dev_assert(team_label != nullptr);
    dev_assert(username_label != nullptr);
    dev_assert(in_game_status_label != nullptr);

    team_label->set_text(team);
    username_label->set_text(username);
    in_game_status_label->set_text(in_game_status);
}

void MultiplayerLobbyPlayer::set_team(String team) { this->team = team; }

void MultiplayerLobbyPlayer::set_username(String username) {
    this->username = username;
}

void MultiplayerLobbyPlayer::set_in_game_status(String in_game_status) {
    this->in_game_status = in_game_status;
}