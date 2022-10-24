#ifndef A2_CAMERA_H
#define A2_CAMERA_H

#include <InputEvent.hpp>
#include <Ref.hpp>
#include <SpringArm.hpp>

#include "../common.h"

class Camera : public SpringArm {
    GODOT_CLASS(Camera, SpringArm);

   private:
    static constexpr float MIN_LENGTH = 3;
    static constexpr float MAX_LENGTH = 20;
    static constexpr float LERP_T = 0.25f;
    static constexpr float DIFF_MIN = 1e-5f;

    float sensitivity;
    float length_delta;
    float length_target;

   public:
    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _input(Ref<InputEvent> event);

    float get_sensitivity();
    void set_sensitivity(float sensitivity);

    float get_length_delta();
    void set_length_delta(float length_delta);

    bool can_function();
};

#endif