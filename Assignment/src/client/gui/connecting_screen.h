#ifndef A2_CONNECTING_SCREEN_H
#define A2_CONNECTING_SCREEN_H

#include <BaseButton.hpp>
#include <Control.hpp>
#include <Label.hpp>
#include <LineEdit.hpp>

#include "../common.h"

class ConnectingScreen : public Control {
    GODOT_CLASS(ConnectingScreen, Control);

   private:
    Label *descrption;
    NodePath descrption_path;

    LineEdit *server_ip;
    NodePath server_ip_path;

    BaseButton *connect_button;
    NodePath connect_button_path;

   public:
    static void _register_methods();
    void _init();

    void _ready();
    void connected_fail();
    void connected_ok();
    void on_connect_button_pressed();
};

#endif