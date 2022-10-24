#ifndef A2_BACKGROUND_MUSIC_H
#define A2_BACKGROUND_MUSIC_H

#include <AudioStreamPlayer.hpp>
#include <InputEvent.hpp>

#include "../common.h"

class BackgroundMusic : public AudioStreamPlayer {
    GODOT_CLASS(BackgroundMusic, AudioStreamPlayer);

   private:
    float boost;
    float sensitivity;

   public:
    static void _register_methods();

    void _init();
    void _ready();
    void _input(Ref<InputEvent> event);
};

#endif