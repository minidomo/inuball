#include "./multiplayer_main_menu.h"

#include <PackedScene.hpp>
#include <Ref.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>

#include "../../debug.h"
#include "../../singletons/client.h"
#include "../../util.h"
#include "./multiplayer_lobby_card.h"

void MultiplayerMainMenu::_register_methods() {
    register_method("_ready", &MultiplayerMainMenu::_ready);

    register_method("on_back_button_pressed",
                    &MultiplayerMainMenu::on_back_button_pressed);
    register_property<MultiplayerMainMenu, NodePath>(
        "back_button_path", &MultiplayerMainMenu::back_button_path, NodePath());

    register_method("on_create_lobby_button_pressed",
                    &MultiplayerMainMenu::on_create_lobby_button_pressed);
    register_property<MultiplayerMainMenu, NodePath>(
        "create_lobby_button_path",
        &MultiplayerMainMenu::create_lobby_button_path, NodePath());

    register_method("on_refresh_button_pressed",
                    &MultiplayerMainMenu::on_refresh_button_pressed);
    register_property<MultiplayerMainMenu, NodePath>(
        "refresh_button_path", &MultiplayerMainMenu::refresh_button_path,
        NodePath());

    register_property<MultiplayerMainMenu, NodePath>(
        "lobby_list_path", &MultiplayerMainMenu::lobby_list_path, NodePath());

    register_method("on_received_all_lobbies",
                    &MultiplayerMainMenu::on_received_all_lobbies);
}

void MultiplayerMainMenu::_init() {
    back_button_path = NodePath();
    create_lobby_button_path = NodePath();
    refresh_button_path = NodePath();
    lobby_list_path = NodePath();
}

void MultiplayerMainMenu::_ready() {
    dev_assert(!back_button_path.is_empty());
    dev_assert(!create_lobby_button_path.is_empty());
    dev_assert(!refresh_button_path.is_empty());
    dev_assert(!lobby_list_path.is_empty());

    back_button = Object::cast_to<BaseButton>(get_node(back_button_path));
    create_lobby_button =
        Object::cast_to<BaseButton>(get_node(create_lobby_button_path));
    refresh_button = Object::cast_to<BaseButton>(get_node(refresh_button_path));
    lobby_list = Object::cast_to<VBoxContainer>(get_node(lobby_list_path));

    dev_assert(back_button != nullptr);
    dev_assert(create_lobby_button != nullptr);
    dev_assert(refresh_button != nullptr);
    dev_assert(lobby_list != nullptr);

    back_button->connect("pressed", this, "on_back_button_pressed");
    create_lobby_button->connect("pressed", this,
                                 "on_create_lobby_button_pressed");
    refresh_button->connect("pressed", this, "on_refresh_button_pressed");

    Client::get_singleton(this)->connect("received_all_lobbies", this,
                                         "on_received_all_lobbies");

    // initially request lobbies from server
    Client::get_singleton(this)->request_all_lobbies();
}

void MultiplayerMainMenu::on_back_button_pressed() {
    get_tree()->change_scene("res://scenes/gui/MainMenu.tscn");
}

void MultiplayerMainMenu::on_create_lobby_button_pressed() {
    get_tree()->change_scene("res://scenes/gui/MultiplayerLobbyCreate.tscn");
}

void MultiplayerMainMenu::on_refresh_button_pressed() {
    Client::get_singleton(this)->request_all_lobbies();
}

void MultiplayerMainMenu::on_received_all_lobbies(Dictionary data) {
    Util::delete_children(lobby_list);

    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load(
        "res://scenes/gui/MultiplayerLobbyCard.tscn");

    Array arr = data.values();

    for (int i = 0; i < arr.size(); i++) {
        Dictionary lobby_card_data = arr[i];
        MultiplayerLobbyCard *card =
            Object::cast_to<MultiplayerLobbyCard>(scene->instance());

        dev_assert(card != nullptr);

        card->set_title(lobby_card_data["title"]);
        card->set_id(lobby_card_data["id"]);
        card->set_current_size(lobby_card_data["current_size"]);
        card->set_max_size(lobby_card_data["max_size"]);
        card->set_status(lobby_card_data["status"]);
        card->set_game_length(lobby_card_data["game_length"]);

        lobby_list->add_child(card);
    }
}