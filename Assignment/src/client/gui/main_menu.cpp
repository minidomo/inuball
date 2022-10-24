#include "./main_menu.h"

#include <SceneTree.hpp>

#include "../debug.h"
#include "../singletons/client.h"
#include "../globals.h"

void MainMenu::_register_methods() {
    register_method("_ready", &MainMenu::_ready);

    register_method("on_singleplayer_button_pressed",
                    &MainMenu::on_singleplayer_button_pressed);
    register_property<MainMenu, NodePath>("singleplayer_button_path",
                                          &MainMenu::singleplayer_button_path,
                                          NodePath());

    register_method("on_multiplayer_button_pressed",
                    &MainMenu::on_multiplayer_button_pressed);
    register_property<MainMenu, NodePath>("multiplayer_button_path",
                                          &MainMenu::multiplayer_button_path,
                                          NodePath());

    register_method("on_exit_button_pressed",
                    &MainMenu::on_exit_button_pressed);
    register_property<MainMenu, NodePath>(
        "exit_button_path", &MainMenu::exit_button_path, NodePath());

    register_method("on_set_username_button_pressed",
                    &MainMenu::on_set_username_button_pressed);
    register_property<MainMenu, NodePath>("set_username_button_path",
                                          &MainMenu::set_username_button_path,
                                          NodePath());

    register_property<MainMenu, NodePath>(
        "username_entry_path", &MainMenu::username_entry_path, NodePath());
}

void MainMenu::_init() {
    singleplayer_button_path = NodePath();
    multiplayer_button_path = NodePath();
    exit_button_path = NodePath();
    set_username_button_path = NodePath();
    username_entry_path = NodePath();
}

void MainMenu::_ready() {
    dev_assert(!singleplayer_button_path.is_empty());
    dev_assert(!multiplayer_button_path.is_empty());
    dev_assert(!exit_button_path.is_empty());
    dev_assert(!set_username_button_path.is_empty());
    dev_assert(!username_entry_path.is_empty());

    singleplayer_button =
        Object::cast_to<BaseButton>(get_node(singleplayer_button_path));
    multiplayer_button =
        Object::cast_to<BaseButton>(get_node(multiplayer_button_path));
    exit_button = Object::cast_to<BaseButton>(get_node(exit_button_path));
    set_username_button =
        Object::cast_to<BaseButton>(get_node(set_username_button_path));
    username_entry = Object::cast_to<LineEdit>(get_node(username_entry_path));

    dev_assert(singleplayer_button != nullptr);
    dev_assert(multiplayer_button != nullptr);
    dev_assert(exit_button != nullptr);
    dev_assert(set_username_button != nullptr);
    dev_assert(username_entry != nullptr);

    singleplayer_button->connect("pressed", this,
                                 "on_singleplayer_button_pressed");
    multiplayer_button->connect("pressed", this,
                                "on_multiplayer_button_pressed");
    exit_button->connect("pressed", this, "on_exit_button_pressed");
    set_username_button->connect("pressed", this,
                                 "on_set_username_button_pressed");

    // make sure username is up to date
    username_entry->set_text(Client::get_singleton(this)->get_username());
}

void MainMenu::on_singleplayer_button_pressed() {
    GLOBALS::instance.multiplayer = false;
    get_tree()->change_scene(
        "res://scenes/playground/CustomWorldPlayground.tscn");
}

void MainMenu::on_multiplayer_button_pressed() {
    GLOBALS::instance.multiplayer = true;
    get_tree()->change_scene("res://scenes/gui/MultiplayerMainMenu.tscn");
}

void MainMenu::on_exit_button_pressed() { get_tree()->quit(); }

void MainMenu::on_set_username_button_pressed() {
    if (username_entry->is_editable()) {
        Client::get_singleton(this)->set_username(username_entry->get_text());
        Client::get_singleton(this)->request_update_username();
        username_entry->set_editable(false);
    } else {
        username_entry->set_editable(true);
    }
}