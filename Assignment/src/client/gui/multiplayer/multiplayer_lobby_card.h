#ifndef A2_MULTIPLAYER_LOBBY_CARD_H
#define A2_MULTIPLAYER_LOBBY_CARD_H

#include <BaseButton.hpp>
#include <Control.hpp>
#include <Label.hpp>

#include "../../common.h"

class MultiplayerLobbyCard : public Control {
    GODOT_CLASS(MultiplayerLobbyCard, Control);

   private:
    BaseButton *join_button;
    NodePath join_button_path;

    Label *title_label;
    NodePath title_label_path;

    Label *id_label;
    NodePath id_label_path;

    Label *current_size_label;
    NodePath current_size_label_path;

    Label *max_size_label;
    NodePath max_size_label_path;

    Label *status_label;
    NodePath status_label_path;

    Label *game_length_label;
    NodePath game_length_label_path;

    String title;
    int64_t id;
    int64_t current_size;
    int64_t max_size;
    String status;
    int64_t game_length;

   public:
    static void _register_methods();
    void _init();
    void _ready();
    void on_join_button_pressed();
    void set_title(String title);
    void set_id(int64_t id);
    void set_current_size(int64_t current_size);
    void set_max_size(int64_t max_size);
    void set_status(String status);
    void set_game_length(int64_t game_length);
};

#endif