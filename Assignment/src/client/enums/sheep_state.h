#ifndef A4_SHEEP_STATE_H
#define A4_SHEEP_STATE_H

enum class SheepState {
    DEFAULT = 0,
    NEAR_PLAYER = 1,
    NEAR_GOAL = 2,
};

constexpr auto operator+(SheepState o) noexcept {
    return static_cast<std::underlying_type_t<SheepState>>(o);
}

#endif