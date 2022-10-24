#include "camera.h"

#include <GlobalConstants.hpp>
#include <Input.hpp>
#include <InputEventMouseButton.hpp>
#include <InputEventMouseMotion.hpp>

void Camera::_register_methods() {
    register_method("_ready", &Camera::_ready);
    register_method("_process", &Camera::_process);
    register_method("_input", &Camera::_input);
    register_property<Camera, float>("sensitivity", &Camera::sensitivity, 0.5f);
    register_property<Camera, float>("length_delta", &Camera::length_delta,
                                     2.0f);
}

void Camera::_init() {
    sensitivity = 0.5f;
    length_delta = 2;
    this->set_margin(0.1f);
}

void Camera::_ready() {
    set_as_toplevel(true);

    length_target = get_length();

    Input *input = Input::get_singleton();
    input->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
}

/**
 * Handles zooming in/out the camera using the SpringArm's length property
 */
void Camera::_process(float delta) {
    if (!can_function()) return;

    // get difference to check if the current length is equal to the target
    // length
    float diff = abs(get_length() - length_target);

    // due to floating point errors, we adjust the length if the difference is
    // within DIFF_MIN
    if (diff >= DIFF_MIN) {
        float length_res = Math::lerp(get_length(), length_target, LERP_T);
        length_res = Math::clamp(length_res, MIN_LENGTH, MAX_LENGTH);
        set_length(length_res);
    }
}

/**
 * Handles rotation movement of camera and zoom input
 */
void Camera::_input(Ref<InputEvent> event) {
    if (!can_function()) return;

    InputEventMouseMotion *motion_event =
        Object::cast_to<InputEventMouseMotion>(event.ptr());
    InputEventMouseButton *button_event =
        Object::cast_to<InputEventMouseButton>(event.ptr());

    if (motion_event) {
        // rotate camera
        // https://www.youtube.com/watch?v=UpF7wm0186Q
        Vector2 relative = motion_event->get_relative();
        Vector3 rot_deg = get_rotation_degrees();

        rot_deg.x -= relative.y * sensitivity;
        rot_deg.x = Math::clamp<float>(rot_deg.x, -90, 90);

        rot_deg.y -= relative.x * sensitivity;
        rot_deg.y = Math::wrapf(rot_deg.y, 0, 360);

        set_rotation_degrees(rot_deg);
    } else if (button_event) {
        // handle zoom
        int64_t index = button_event->get_button_index();

        if (index == GlobalConstants::BUTTON_WHEEL_UP) {
            // zoom in
            length_target = get_length() - length_delta;
            length_target = Math::clamp(length_target, MIN_LENGTH, MAX_LENGTH);
        } else if (index == GlobalConstants::BUTTON_WHEEL_DOWN) {
            // zoom out
            length_target = get_length() + length_delta;
            length_target = Math::clamp(length_target, MIN_LENGTH, MAX_LENGTH);
        }
    }
}

/**
 * @returns the sensitivity of camera rotation
 */
float Camera::get_sensitivity() { return sensitivity; }

/**
 * Sets the sensitivity of camera rotation
 */
void Camera::set_sensitivity(float sensitivity) {
    this->sensitivity = sensitivity;
}

/**
 * @returns the amount to zoom in/out per scroll
 */
float Camera::get_length_delta() { return length_delta; }

/**
 * Sets the amount to zoom in/out per scroll
 */
void Camera::set_length_delta(float length_delta) {
    this->length_delta = length_delta;
}

/**
 * Checks if the camera is in a state where it can function
 * @returns true if camera is in a state where it can function, false otherwise
 */
bool Camera::can_function() {
    Input *input = Input::get_singleton();
    if (input->get_mouse_mode() != Input::MOUSE_MODE_CAPTURED) return false;

    return true;
}
