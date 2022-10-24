#ifndef A2_MAIN_MENU_H
#define A2_MAIN_MENU_H

#include <BaseButton.hpp>
#include <Control.hpp>
#include <LineEdit.hpp>

#include "../common.h"

class MainMenu : public Control {
    GODOT_CLASS(MainMenu, Control);

   private:
    BaseButton *singleplayer_button;
    NodePath singleplayer_button_path;

    BaseButton *multiplayer_button;
    NodePath multiplayer_button_path;

    BaseButton *exit_button;
    NodePath exit_button_path;

    BaseButton *set_username_button;
    NodePath set_username_button_path;

    LineEdit *username_entry;
    NodePath username_entry_path;

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void on_singleplayer_button_pressed();
    void on_multiplayer_button_pressed();
    void on_exit_button_pressed();
    void on_set_username_button_pressed();
};

#endif