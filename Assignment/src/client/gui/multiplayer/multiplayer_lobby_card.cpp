#include "./multiplayer_lobby_card.h"

#include "../../debug.h"
#include "../../singletons/client.h"
#include "../../util.h"

void MultiplayerLobbyCard::_register_methods() {
    register_method("_ready", &MultiplayerLobbyCard::_ready);

    register_method("on_join_button_pressed",
                    &MultiplayerLobbyCard::on_join_button_pressed);
    register_property<MultiplayerLobbyCard, NodePath>(
        "join_button_path", &MultiplayerLobbyCard::join_button_path,
        NodePath());

    register_property<MultiplayerLobbyCard, NodePath>(
        "title_label_path", &MultiplayerLobbyCard::title_label_path,
        NodePath());

    register_property<MultiplayerLobbyCard, NodePath>(
        "id_label_path", &MultiplayerLobbyCard::id_label_path, NodePath());

    register_property<MultiplayerLobbyCard, NodePath>(
        "current_size_label_path",
        &MultiplayerLobbyCard::current_size_label_path, NodePath());

    register_property<MultiplayerLobbyCard, NodePath>(
        "max_size_label_path", &MultiplayerLobbyCard::max_size_label_path,
        NodePath());

    register_property<MultiplayerLobbyCard, NodePath>(
        "status_label_path", &MultiplayerLobbyCard::status_label_path,
        NodePath());

    register_property<MultiplayerLobbyCard, NodePath>(
        "game_length_label_path", &MultiplayerLobbyCard::game_length_label_path,
        NodePath());
}

void MultiplayerLobbyCard::_init() {
    join_button_path = NodePath();
    title_label_path = NodePath();
    id_label_path = NodePath();
    current_size_label_path = NodePath();
    max_size_label_path = NodePath();
    status_label_path = NodePath();
    game_length_label_path = NodePath();
}

void MultiplayerLobbyCard::_ready() {
    dev_assert(!join_button_path.is_empty());
    dev_assert(!title_label_path.is_empty());
    dev_assert(!id_label_path.is_empty());
    dev_assert(!current_size_label_path.is_empty());
    dev_assert(!max_size_label_path.is_empty());
    dev_assert(!status_label_path.is_empty());
    dev_assert(!game_length_label_path.is_empty());

    join_button = Object::cast_to<BaseButton>(get_node(join_button_path));
    title_label = Object::cast_to<Label>(get_node(title_label_path));
    id_label = Object::cast_to<Label>(get_node(id_label_path));
    current_size_label =
        Object::cast_to<Label>(get_node(current_size_label_path));
    max_size_label = Object::cast_to<Label>(get_node(max_size_label_path));
    status_label = Object::cast_to<Label>(get_node(status_label_path));
    game_length_label =
        Object::cast_to<Label>(get_node(game_length_label_path));

    dev_assert(join_button != nullptr);
    dev_assert(title_label != nullptr);
    dev_assert(id_label != nullptr);
    dev_assert(current_size_label != nullptr);
    dev_assert(max_size_label != nullptr);
    dev_assert(status_label != nullptr);
    dev_assert(game_length_label != nullptr);

    join_button->connect("pressed", this, "on_join_button_pressed");

    title_label->set_text(title);
    id_label->set_text(String::num_int64(id));
    current_size_label->set_text(String::num_int64(current_size));
    max_size_label->set_text(String::num_int64(max_size));
    status_label->set_text(status);
    game_length_label->set_text(Util::get_time_str(game_length));
}

void MultiplayerLobbyCard::on_join_button_pressed() {
    Client::get_singleton(this)->request_join_lobby(id);
}

void MultiplayerLobbyCard::set_title(String title) { this->title = title; }

void MultiplayerLobbyCard::set_id(int64_t id) { this->id = id; }

void MultiplayerLobbyCard::set_current_size(int64_t current_size) {
    this->current_size = current_size;
}

void MultiplayerLobbyCard::set_max_size(int64_t max_size) {
    this->max_size = max_size;
}

void MultiplayerLobbyCard::set_status(String status) { this->status = status; }

void MultiplayerLobbyCard::set_game_length(int64_t game_length) {
    this->game_length = game_length;
}