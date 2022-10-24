#include "./multiplayer_lobby_create_setting.h"

#include "../../debug.h"

void MultiplayerLobbyCreateSetting::_register_methods() {
    register_method("_ready", &MultiplayerLobbyCreateSetting::_ready);

    register_property<MultiplayerLobbyCreateSetting, String>(
        "setting_name", &MultiplayerLobbyCreateSetting::setting_name,
        String("Setting"));

    register_property<MultiplayerLobbyCreateSetting, NodePath>(
        "name_label_path", &MultiplayerLobbyCreateSetting::name_label_path,
        NodePath());

    register_property<MultiplayerLobbyCreateSetting, NodePath>(
        "value_text_edit_path",
        &MultiplayerLobbyCreateSetting::value_line_edit_path, NodePath());
}

void MultiplayerLobbyCreateSetting::_init() {
    setting_name = String("Setting");
    name_label_path = NodePath();
    value_line_edit_path = NodePath();
}

void MultiplayerLobbyCreateSetting::_ready() {
    dev_assert(!name_label_path.is_empty());
    dev_assert(!value_line_edit_path.is_empty());

    name_label = Object::cast_to<Label>(get_node(name_label_path));
    value_line_edit = Object::cast_to<LineEdit>(get_node(value_line_edit_path));

    dev_assert(name_label != nullptr);
    dev_assert(value_line_edit != nullptr);

    name_label->set_text(setting_name);
}

String MultiplayerLobbyCreateSetting::get_value() {
    return value_line_edit->get_text();
}