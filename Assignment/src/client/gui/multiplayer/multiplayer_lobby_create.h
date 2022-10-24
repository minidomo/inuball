#ifndef A2_MULTIPLAYER_LOBBY_CREATE_H
#define A2_MULTIPLAYER_LOBBY_CREATE_H

#include <BaseButton.hpp>
#include <Control.hpp>

#include "../../common.h"
#include "./multiplayer_lobby_create_setting.h"

class MultiplayerLobbyCreate : public Control {
    GODOT_CLASS(MultiplayerLobbyCreate, Control);

   private:
    static const int MAX_LOBBY_SIZE = 12;
    static const int MIN_LOBBY_SIZE = 2;

    static const int MAX_GAME_LENGTH = 360;
    static const int MIN_GAME_LENGTH = 10;

    BaseButton *cancel_button;
    NodePath cancel_button_path;

    BaseButton *create_button;
    NodePath create_button_path;

    MultiplayerLobbyCreateSetting *title_setting;
    NodePath title_setting_path;

    MultiplayerLobbyCreateSetting *lobby_size_setting;
    NodePath lobby_size_setting_path;

    MultiplayerLobbyCreateSetting *game_length_setting;
    NodePath game_length_setting_path;

   public:
    static void _register_methods();
    void _init();
    void _ready();
    void on_cancel_button_pressed();
    void on_create_button_pressed();
    void on_received_create_lobby(int64_t lobby_id);
};

#endif