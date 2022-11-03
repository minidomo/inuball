#ifndef A4_CHICKEN_STATE_H
#define A4_CHICKEN_STATE_H

enum class ChickenState {
    DEFAULT = 0,
    NEAR_PLAYER_OR_GOAL = 1,
    HIDE = 2,
};

constexpr auto operator+(ChickenState o) noexcept {
    return static_cast<std::underlying_type_t<ChickenState>>(o);
}

#endif