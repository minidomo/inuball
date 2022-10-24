#ifndef A2_MULTIPLAYER_LOBBY_PLAYER_H
#define A2_MULTIPLAYER_LOBBY_PLAYER_H

#include <Control.hpp>
#include <Label.hpp>

#include "../../common.h"

class MultiplayerLobbyPlayer : public Control {
    GODOT_CLASS(MultiplayerLobbyPlayer, Control);

   private:
    Label *team_label;
    NodePath team_label_path;

    Label *username_label;
    NodePath username_label_path;

    Label *in_game_status_label;
    NodePath in_game_status_label_path;

    String team;
    String username;
    String in_game_status;

   public:
    static void _register_methods();
    void _init();
    void _ready();
    void set_team(String team);
    void set_username(String username);
    void set_in_game_status(String in_game_status);
};

#endif