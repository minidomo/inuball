#ifndef A2_MULTIPLAYER_LOBBY_H
#define A2_MULTIPLAYER_LOBBY_H

#include <BaseButton.hpp>
#include <Control.hpp>
#include <Label.hpp>
#include <VBoxContainer.hpp>

#include "../../common.h"

class MultiplayerLobby : public Control {
    GODOT_CLASS(MultiplayerLobby, Control);

   private:
    BaseButton *start_button;
    NodePath start_button_path;

    BaseButton *leave_button;
    NodePath leave_button_path;

    BaseButton *change_team_button;
    NodePath change_team_button_path;

    Label *title_label;
    NodePath title_label_path;

    Label *lobby_size_label;
    NodePath lobby_size_label_path;

    Label *game_length_label;
    NodePath game_length_label_path;

    VBoxContainer *player_list;
    NodePath player_list_path;

   public:
    static void _register_methods();
    void _init();
    void _ready();
    void on_start_button_pressed();
    void on_leave_button_pressed();
    void on_change_team_button_pressed();
    void update_scene();
};

#endif