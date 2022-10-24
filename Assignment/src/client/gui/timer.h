#ifndef A2_TIMER_H
#define A2_TIMER_H

#include <HBoxContainer.hpp>
#include <InputEvent.hpp>
#include <Label.hpp>
#include <PackedScene.hpp>
#include <Texture.hpp>
#include <TextureRect.hpp>
#include <Timer.hpp>

#include "../common.h"

class GUITimer : public HBoxContainer {
    GODOT_CLASS(GUITimer, HBoxContainer);

   private:
    int game_length;
    int16_t time;
    Label *time_label = nullptr;
    Timer *timer = nullptr;

   public:
    static void _register_methods();

    void _init();
    void _ready();
    void tick();
    void play_again();
    void set_time(int16_t time);
    int16_t get_time();
    void exit_game();
};

#endif