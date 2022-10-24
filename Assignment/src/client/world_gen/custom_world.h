
#ifndef A2_WORLD_H
#define A2_WORLD_H

// Borrowed heavily from here: https://www.youtube.com/watch?v=Q2iWDNq5PaU

#include <OpenSimplexNoise.hpp>

#include "chunk.h"

class CustomWorld : public StaticBody {
    GODOT_CLASS(CustomWorld, StaticBody)

   public:
   private:
    static constexpr int32_t convolution = 2;

    float time_passed;

    Chunk *current = nullptr;
    Spatial *player = nullptr;
    Node *players = nullptr;

    OpenSimplexNoise *noise = OpenSimplexNoise::_new();

    NodePath player_path;
    NodePath players_path;

   public:
    static void _register_methods();

    CustomWorld();
    ~CustomWorld();

    void _init();
    void _ready();

    void _process(float delta);
    void spawn_player(Dictionary info, uint32_t self_id);
    void _update_player_state(uint64_t client_id, Vector3 translation,
                              Vector3 rotation);
    void _update_stats(uint64_t timer, uint64_t red_chicken, uint64_t red_sheep,
                       uint64_t blue_chicken, uint64_t blue_sheep);
    void _get_update_stats(uint64_t client_id, uint64_t lobby_id);

    friend class Player;
};

#endif
