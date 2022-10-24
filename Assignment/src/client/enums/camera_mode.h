#ifndef A2_CAMERA_MODE_H
#define A2_CAMERA_MODE_H

enum class CameraMode {
    DEFAULT,
    FREE,
};

// https://www.learncpp.com/cpp-tutorial/scoped-enumerations-enum-classes/#operatorplus
constexpr auto operator+(CameraMode o) noexcept {
    return static_cast<std::underlying_type_t<CameraMode>>(o);
}

#endif