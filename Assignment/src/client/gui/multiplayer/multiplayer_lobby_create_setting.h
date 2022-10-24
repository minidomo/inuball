#ifndef A2_MULTIPLAYER_LOBBY_CREATE_SETTING_H
#define A2_MULTIPLAYER_LOBBY_CREATE_SETTING_H

#include <Control.hpp>
#include <Label.hpp>
#include <LineEdit.hpp>

#include "../../common.h"

class MultiplayerLobbyCreateSetting : public Control {
    GODOT_CLASS(MultiplayerLobbyCreateSetting, Control);

   private:
    String setting_name;

    Label *name_label;
    NodePath name_label_path;

    LineEdit *value_line_edit;
    NodePath value_line_edit_path;

   public:
    static void _register_methods();
    void _init();
    void _ready();
    String get_value();
};

#endif