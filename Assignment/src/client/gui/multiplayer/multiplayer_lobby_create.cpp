#include "./multiplayer_lobby_create.h"

#include <SceneTree.hpp>

#include "../../debug.h"
#include "../../singletons/client.h"
#include "../../util.h"

void MultiplayerLobbyCreate::_register_methods() {
    register_method("_ready", &MultiplayerLobbyCreate::_ready);

    register_method("on_cancel_button_pressed",
                    &MultiplayerLobbyCreate::on_cancel_button_pressed);
    register_property<MultiplayerLobbyCreate, NodePath>(
        "cancel_button_path", &MultiplayerLobbyCreate::cancel_button_path,
        NodePath());

    register_method("on_create_button_pressed",
                    &MultiplayerLobbyCreate::on_create_button_pressed);
    register_property<MultiplayerLobbyCreate, NodePath>(
        "create_button_path", &MultiplayerLobbyCreate::create_button_path,
        NodePath());

    register_property<MultiplayerLobbyCreate, NodePath>(
        "title_setting_path", &MultiplayerLobbyCreate::title_setting_path,
        NodePath());

    register_property<MultiplayerLobbyCreate, NodePath>(
        "lobby_size_setting_path",
        &MultiplayerLobbyCreate::lobby_size_setting_path, NodePath());

    register_property<MultiplayerLobbyCreate, NodePath>(
        "game_length_setting_path",
        &MultiplayerLobbyCreate::game_length_setting_path, NodePath());

    register_method("on_received_create_lobby",
                    &MultiplayerLobbyCreate::on_received_create_lobby);
}

void MultiplayerLobbyCreate::_init() {
    cancel_button_path = NodePath();
    create_button_path = NodePath();
    title_setting_path = NodePath();
    lobby_size_setting_path = NodePath();
    game_length_setting_path = NodePath();
}

void MultiplayerLobbyCreate::_ready() {
    dev_assert(!cancel_button_path.is_empty());
    dev_assert(!create_button_path.is_empty());
    dev_assert(!title_setting_path.is_empty());
    dev_assert(!lobby_size_setting_path.is_empty());
    dev_assert(!game_length_setting_path.is_empty());

    cancel_button = Object::cast_to<BaseButton>(get_node(cancel_button_path));
    create_button = Object::cast_to<BaseButton>(get_node(create_button_path));
    title_setting = Object::cast_to<MultiplayerLobbyCreateSetting>(
        get_node(title_setting_path));
    lobby_size_setting = Object::cast_to<MultiplayerLobbyCreateSetting>(
        get_node(lobby_size_setting_path));
    game_length_setting = Object::cast_to<MultiplayerLobbyCreateSetting>(
        get_node(game_length_setting_path));

    dev_assert(cancel_button != nullptr);
    dev_assert(create_button != nullptr);
    dev_assert(title_setting != nullptr);
    dev_assert(lobby_size_setting != nullptr);
    dev_assert(game_length_setting != nullptr);

    cancel_button->connect("pressed", this, "on_cancel_button_pressed");
    create_button->connect("pressed", this, "on_create_button_pressed");

    Client::get_singleton(this)->connect("received_create_lobby", this,
                                         "on_received_create_lobby");
}

void MultiplayerLobbyCreate::on_cancel_button_pressed() {
    get_tree()->change_scene("res://scenes/gui/MultiplayerMainMenu.tscn");
}

void MultiplayerLobbyCreate::on_create_button_pressed() {
    // check if lobby settings are valid
    if (title_setting->get_value().empty()) return;
    if (!Util::is_only_digits(lobby_size_setting->get_value())) return;
    if (!Util::is_only_digits(game_length_setting->get_value())) return;

    int64_t lobby_size = lobby_size_setting->get_value().to_int();
    if (lobby_size < MIN_LOBBY_SIZE || lobby_size > MAX_LOBBY_SIZE) return;

    int64_t game_length = game_length_setting->get_value().to_int();
    if (game_length < MIN_GAME_LENGTH || game_length > MAX_GAME_LENGTH) return;

    Dictionary data;
    data["title"] = title_setting->get_value();
    data["max_size"] = lobby_size;
    data["game_length"] = game_length;

    // create lobby by requesting to server
    Client::get_singleton(this)->request_create_lobby(data);
}

void MultiplayerLobbyCreate::on_received_create_lobby(int64_t lobby_id) {
    Client::get_singleton(this)->request_join_lobby(lobby_id);
}
