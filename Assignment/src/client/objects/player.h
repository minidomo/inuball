#ifndef A2_PLAYER_H
#define A2_PLAYER_H

#include <AudioStreamPlayer.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <KinematicBody.hpp>
#include <RayCast.hpp>
#include <Ref.hpp>

#include "../common.h"
#include "../enums/camera_mode.h"
#include "../enums/ledge_mode.h"
#include "../enums/player_state.h"
#include "camera.h"
#include "igameobject.h"
#include "ledge_detector.h"
#include "signals/looking_at_receiver.h"

class Player : public KinematicBody {
    GODOT_CLASS(Player, KinematicBody);

   private:
    int64_t id;
    String username;

    float speed;
    float gravity;
    float jump_force;
    float boost_force;
    float boost_decay;
    float boost;
    float glide_air_resistance;
    float air_control;
    float max_angle_walk;
    float time_since_touched;

    Vector3 snap;

    LedgeDetector *ledge_detector;
    NodePath ledge_detector_path;

    LedgeMode ledge_mode;
    CameraMode camera_mode;
    PlayerState player_state;

    AudioStreamPlayer *hit_sound;
    AudioStreamPlayer *miss_sound;
    AudioStreamPlayer *walk_sound;
    AudioStreamPlayer *jump_sound;
    AudioStreamPlayer *dash_sound;

    Camera *camera;
    NodePath camera_path;

    Vector3 ledge_hang_old_position;

    bool sfxOn;
    float sfxDb;
    float sfxSens;

    bool controllable;

    void handle_ledge_stop();
    void handle_ledge_hang();
    void adapt_rotation();
    RayCast *looking_at;

    Node *currently_looking_at = nullptr;
    iGameObject *selected = nullptr;

    Vector3 get_movement_dir();
    void handle_jump();
    void update_boost();
    void handle_glide(float delta);

   public:
    Vector3 velocity;
    bool need_to_handle_collision = false;
    static void _register_methods();

    void set_selected(iGameObject *obj) { selected = obj; }

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
    void set_id(int64_t id);
    int64_t get_id();
    void set_username(String username);
    String get_username();
    void update_remote_player(Vector3 loc);
    void set_controllable(bool cont);

    void touch() { this->time_since_touched = 0; }
    bool handle_others(float delta);

    void _input(Ref<InputEvent> event);

    void body_entered(Node *body);
};

#endif