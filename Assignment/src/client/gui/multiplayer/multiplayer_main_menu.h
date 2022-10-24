#ifndef A2_MULTIPLAYER_MAIN_MENU_H
#define A2_MULTIPLAYER_MAIN_MENU_H

#include <BaseButton.hpp>
#include <Control.hpp>
#include <VBoxContainer.hpp>

#include "../../common.h"

class MultiplayerMainMenu : public Control {
    GODOT_CLASS(MultiplayerMainMenu, Control);

   private:
    BaseButton *back_button;
    NodePath back_button_path;

    BaseButton *create_lobby_button;
    NodePath create_lobby_button_path;

    BaseButton *refresh_button;
    NodePath refresh_button_path;

    VBoxContainer *lobby_list;
    NodePath lobby_list_path;

   public:
    static void _register_methods();
    void _init();
    void _ready();
    void on_back_button_pressed();
    void on_create_lobby_button_pressed();
    void on_refresh_button_pressed();

    void on_received_all_lobbies(Dictionary data);
};

#endif