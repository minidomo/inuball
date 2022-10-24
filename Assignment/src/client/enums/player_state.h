#ifndef A2_PLAYER_STATE_H
#define A2_PLAYER_STATE_H

enum class PlayerState {
    DEFAULT,
    HANGING,
};

constexpr auto operator+(PlayerState o) noexcept {
    return static_cast<std::underlying_type_t<PlayerState>>(o);
}

#endif