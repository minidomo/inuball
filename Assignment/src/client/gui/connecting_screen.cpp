#include "./connecting_screen.h"

#include <SceneTree.hpp>

#include "../debug.h"
#include "../singletons/client.h"

void ConnectingScreen::_register_methods() {
    register_method("_ready", &ConnectingScreen::_ready);

    register_property<ConnectingScreen, NodePath>(
        "description_path", &ConnectingScreen::descrption_path, NodePath());
    register_property<ConnectingScreen, NodePath>(
        "server_ip_path", &ConnectingScreen::server_ip_path, NodePath());
    register_property<ConnectingScreen, NodePath>(
        "connect_button_path", &ConnectingScreen::connect_button_path,
        NodePath());

    register_method("connected_ok", &ConnectingScreen::connected_ok);
    register_method("connected_fail", &ConnectingScreen::connected_fail);
    register_method("on_connect_button_pressed",
                    &ConnectingScreen::on_connect_button_pressed);
}

void ConnectingScreen::_init() {
    descrption_path = NodePath();
    server_ip_path = NodePath();
    connect_button_path = NodePath();
}

void ConnectingScreen::_ready() {
    dev_assert(!descrption_path.is_empty());
    dev_assert(!server_ip_path.is_empty());
    dev_assert(!connect_button_path.is_empty());

    descrption = Object::cast_to<Label>(get_node(descrption_path));
    server_ip = Object::cast_to<LineEdit>(get_node(server_ip_path));
    connect_button = Object::cast_to<BaseButton>(get_node(connect_button_path));

    dev_assert(descrption != nullptr);
    dev_assert(server_ip != nullptr);
    dev_assert(connect_button != nullptr);

    connect_button->connect("pressed", this, "on_connect_button_pressed");

    get_tree()->connect("connected_to_server", this, "connected_ok");
    get_tree()->connect("connection_failed", this, "connected_fail");
}

void ConnectingScreen::connected_ok() {
    get_tree()->change_scene("res://scenes/gui/MainMenu.tscn");
}

void ConnectingScreen::connected_fail() {
    String failed_text =
        "Could not connect to server.\nPlease restart with an active server.";
    descrption->set_text(failed_text);
}

void ConnectingScreen::on_connect_button_pressed() {
    String connecting_text = "Connecting to server...";
    descrption->set_text(connecting_text);
    Client::get_singleton(this)->connect_to_server(server_ip->get_text());
}