#ifndef A2_LEDGE_MODE_H
#define A2_LEDGE_MODE_H

enum class LedgeMode {
    DEFAULT,
    STOP,
};

// https://www.learncpp.com/cpp-tutorial/scoped-enumerations-enum-classes/#operatorplus
constexpr auto operator+(LedgeMode o) noexcept {
    return static_cast<std::underlying_type_t<LedgeMode>>(o);
}

#endif