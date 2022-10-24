#include "background_music.h"

#include <GlobalConstants.hpp>
#include <InputEventKey.hpp>

void BackgroundMusic::_register_methods() {
    register_method("_ready", &BackgroundMusic::_ready);
    register_method("_input", &BackgroundMusic::_input);
}

void BackgroundMusic::_init() {
    boost = 0.0;
    sensitivity = 2.0;
}

void BackgroundMusic::_ready() {}

void BackgroundMusic::_input(Ref<InputEvent> event) {
    InputEventKey *key_event = Object::cast_to<InputEventKey>(event.ptr());
    if (key_event) {
        int64_t key = key_event->get_scancode();
        if (key == GlobalConstants::KEY_I && key_event->is_pressed()) {
            Godot::print("BGM Up");
            boost += sensitivity;
            set_volume_db(boost);
        } else if (key == GlobalConstants::KEY_K && key_event->is_pressed()) {
            Godot::print("BGM Down");
            boost -= sensitivity;
            set_volume_db(boost);
        } else if (key == GlobalConstants::KEY_O && key_event->is_pressed()) {
            set_stream_paused(!get_stream_paused());
        }
    }
}