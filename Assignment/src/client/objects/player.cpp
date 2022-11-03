#include "player.h"

#include <GlobalConstants.hpp>
#include <InputEventKey.hpp>
#include <Math.hpp>

#include "../debug.h"
#include "../singletons/client.h"
#include "../util.h"

void Player::_register_methods() {
    register_method("_ready", &Player::_ready);
    register_method("_process", &Player::_process);
    register_method("_physics_process", &Player::_physics_process);
    register_method("_input", &Player::_input);
    register_method("set_controllable", &Player::set_controllable);
    register_method("set_id", &Player::set_id);
    register_method("set_username", &Player::set_username);
    register_method("get_id", &Player::get_id);
    register_method("get_username", &Player::get_username);
    register_method("body_entered", &Player::body_entered);

    register_property<Player, float>("Speed", &Player::speed, 10.0);
    register_property<Player, float>("Jump Force", &Player::jump_force, 30.0);
    register_property<Player, float>("Gravity", &Player::gravity, 50.0);
    register_property<Player, float>("Max Slope Angle", &Player::max_angle_walk,
                                     50.0);
    register_property<Player, float>("Boost Force", &Player::boost_force, 10.0);
    register_property<Player, float>("Boost Decay", &Player::boost_decay, 10.0);
    register_property<Player, float>("Glide Air Resistance",
                                     &Player::glide_air_resistance, 45.0);

    register_property<Player, NodePath>(
        "ledge_detector_path", &Player::ledge_detector_path, NodePath());
    register_property<Player, NodePath>("camera_path", &Player::camera_path,
                                        NodePath());
    register_property<Player, float>("Air Control", &Player::air_control, 0.5);

    register_signal<Player>("looking_at", "node", (Node *)nullptr, "point",
                            Vector3(0, 0, 0), "normal", Vector3(0, 0, 0),
                            "distance", 0.0f);
}

void Player::_init() {
    speed = 10.0;
    jump_force = 30.0;
    gravity = 50.0;
    boost_force = 10.0;
    boost_decay = 10.0;
    boost = 1;
    glide_air_resistance = 45.0;
    max_angle_walk = 50.0;

    sfxOn = true;
    sfxDb = 0.0;
    sfxSens = 1.0;

    snap = Vector3::ZERO;
    velocity = Vector3::ZERO;

    ledge_detector_path = NodePath();
    camera_path = NodePath();
    ledge_mode = LedgeMode::DEFAULT;
    camera_mode = CameraMode::DEFAULT;
    player_state = PlayerState::DEFAULT;
    air_control = 0.5;

    controllable = true;
}

void recursiveRayCastExceptions(RayCast *ray, Node *node) {
    ray->add_exception(node);
    for (int i = 0; i < node->get_child_count(); i++) {
        recursiveRayCastExceptions(ray, node->get_child(i));
    }
}

void Player::_ready() {
    dev_assert(!ledge_detector_path.is_empty());
    ledge_detector =
        Object::cast_to<LedgeDetector>(get_node(ledge_detector_path));
    dev_assert(ledge_detector != nullptr);

    dev_assert(!camera_path.is_empty());
    camera = Object::cast_to<Camera>(get_node(camera_path));
    dev_assert(camera != nullptr);

    Vector3 rot = camera->get_rotation_degrees();
    rot.x = 0;
    rot.y = Math::wrapf(rot.y + 180, 0, 360);
    set_rotation_degrees(rot);
    looking_at = Object::cast_to<RayCast>(camera->get_node("looking_at"));
    hit_sound = Object::cast_to<AudioStreamPlayer>(get_node("AttackSound"));
    miss_sound = Object::cast_to<AudioStreamPlayer>(get_node("MissSound"));
    jump_sound = Object::cast_to<AudioStreamPlayer>(get_node("JumpSound"));
    dash_sound = Object::cast_to<AudioStreamPlayer>(get_node("DashSound"));
    walk_sound = Object::cast_to<AudioStreamPlayer>(get_node("WalkSound"));
    recursiveRayCastExceptions(looking_at, this);

    this->get_node("./Area")->connect("body_entered", this, "body_entered");
}

void Player::body_entered(Node *body) {
    if (body->has_method("collide_with_player"))
        body->call("collide_with_player", this);
}

bool Player::handle_others(float delta) {
    if (controllable) return false;
    this->time_since_touched += delta;
    if (this->time_since_touched > 1.0f) this->queue_free();
    return true;
}

void Player::_process(float delta) {
    if (handle_others(delta)) return;
    adapt_rotation();
    camera->set_translation(get_translation());
}

void Player::_physics_process(float delta) {
    if (!controllable) {
        return;
    }
    if (need_to_handle_collision) {
        velocity = move_and_slide_with_snap(velocity, snap, Vector3::UP, true,
                                            4, Math::deg2rad(max_angle_walk));
        if (velocity.length_squared() < 1) need_to_handle_collision = false;
        velocity.y -= gravity * delta;
        velocity *= 0.9;
        // return;
    }
    if (player_state == PlayerState::DEFAULT) {
        Vector3 move_dir = get_movement_dir();

        if (walk_sound->is_playing() &&
            (!is_on_floor() || move_dir.length_squared() == 0)) {
            walk_sound->stop();
        }

        if (!is_on_floor()) {
            move_dir *= air_control;
        } else if ((move_dir.x != 0 || move_dir.z != 0) &&
                   !walk_sound->is_playing()) {
            walk_sound->play();
        }

        update_boost();
        velocity.x = move_dir.x * speed * boost;
        velocity.z = move_dir.z * speed * boost;
        velocity.y -= gravity * delta;

        handle_jump();
        handle_glide(delta);
        handle_ledge_stop();
        handle_ledge_hang();

        velocity = move_and_slide_with_snap(velocity, snap, Vector3::UP, true,
                                            4, Math::deg2rad(max_angle_walk));
    } else if (player_state == PlayerState::HANGING) {
        handle_ledge_hang();
    }

    auto node = Object::cast_to<Node>(looking_at->get_collider());
    if (node && node != currently_looking_at) {
        currently_looking_at = node;
        selected = nullptr;
        this->emit_signal(
            "looking_at", this, node, looking_at->get_collision_point(),
            looking_at->get_collision_normal(),
            (looking_at->get_collision_point() - this->get_translation() +
             looking_at->get_translation())
                .length());
    }

    Client::get_singleton(this)->update_player(get_translation(),
                                               get_rotation());
}

void Player::_input(Ref<InputEvent> event) {
    if (!controllable) {
        return;
    }
    InputEvent *input = event.ptr();

    if (player_state == PlayerState::DEFAULT) {
        if (input->is_action_pressed("player_toggle_ledge")) {
            // toggle ledge mode
            switch (ledge_mode) {
                case LedgeMode::DEFAULT: {
                    ledge_mode = LedgeMode::STOP;
                    break;
                }
                case LedgeMode::STOP: {
                    ledge_mode = LedgeMode::DEFAULT;
                    break;
                }
            }

            Godot::print("ledge mode: " + String::num_int64(+ledge_mode));
        }
    } else if (player_state == PlayerState::HANGING) {
        if (input->is_action_pressed("player_ledge_release")) {
            // update playerstate
            player_state = PlayerState::DEFAULT;
        }
    }

    if (input->is_action_pressed("player_free_look")) {
        // toggle camera mode
        switch (camera_mode) {
            case CameraMode::DEFAULT: {
                camera_mode = CameraMode::FREE;
                break;
            }
            case CameraMode::FREE: {
                camera_mode = CameraMode::DEFAULT;
                break;
            }
        }
    }

    if (Input::get_singleton()->is_action_just_pressed("click_secondary")) {
        if (!(selected && selected->interact_secondary(this))) {
            miss_sound->play();
        }
    }
    if (Input::get_singleton()->is_action_just_pressed("click_primary")) {
        // TODO play attack sound
        hit_sound->play();
        if (selected) selected->interact_primary(this);
    }

    // handle sfx for now
    InputEventKey *key_event = Object::cast_to<InputEventKey>(event.ptr());
    if (key_event) {
        int64_t key = key_event->get_scancode();
        if (key == GlobalConstants::KEY_U && key_event->is_pressed()) {
            sfxDb += sfxSens;
            hit_sound->set_volume_db(sfxDb);
            miss_sound->set_volume_db(sfxDb);
            jump_sound->set_volume_db(sfxDb);
            dash_sound->set_volume_db(sfxDb);
            walk_sound->set_volume_db(sfxDb);
        } else if (key == GlobalConstants::KEY_J && key_event->is_pressed()) {
            sfxDb -= sfxSens;
            hit_sound->set_volume_db(sfxDb);
            miss_sound->set_volume_db(sfxDb);
            jump_sound->set_volume_db(sfxDb);
            dash_sound->set_volume_db(sfxDb);
            walk_sound->set_volume_db(sfxDb);
        } else if (key == GlobalConstants::KEY_P && key_event->is_pressed()) {
            hit_sound->set_stream_paused(!hit_sound->get_stream_paused());
            miss_sound->set_stream_paused(!miss_sound->get_stream_paused());
            jump_sound->set_stream_paused(!jump_sound->get_stream_paused());
            dash_sound->set_stream_paused(!dash_sound->get_stream_paused());
            walk_sound->set_stream_paused(!walk_sound->get_stream_paused());
        }
    }
}

void Player::handle_ledge_stop() {
    if (ledge_mode != LedgeMode::STOP || !is_on_floor()) return;

    vector<Vector3> ledges =
        ledge_detector->find_ledges(velocity, Array::make(this));

    // no ledges so no need to modify velocity
    if (ledges.empty()) return;

    Vector3 player_origin = get_global_transform().get_origin();
    player_origin.y = 0;

    // entity is not moving horizontally
    Vector3 copy(velocity.x, 0, velocity.z);
    if (copy == Vector3::ZERO) return;

    Vector3 projected;

    for (Vector3 ledge_origin : ledges) {
        Vector3 ledge_normal = ledge_origin.direction_to(player_origin);

        if (Util::vector3_sign(ledge_normal) != Util::vector3_sign(copy)) {
            // https://gamedev.stackexchange.com/a/4060
            Vector3 cur_projected = copy.project(ledge_normal);
            projected += cur_projected;
        }
    }

    velocity -= projected;
}

void Player::handle_ledge_hang() {
    if (ledge_mode != LedgeMode::DEFAULT) return;

    Input *input = Input::get_singleton();

    if (input->is_action_just_pressed("player_ledge_hang")) {
        if (player_state == PlayerState::DEFAULT) {
            if (!is_on_floor()) return;

            Vector3 new_position = ledge_detector->find_ledge_hangable(
                get_global_transform().basis.z, Array::make(this));

            if (new_position == Vector3::INF) return;

            // save current position
            ledge_hang_old_position = get_translation();

            // set new position
            set_translation(new_position);

            // update player state
            player_state = PlayerState::HANGING;
        } else if (player_state == PlayerState::HANGING) {
            // this must go here since they use the same key. _input will react
            // faster than _physics_process so this will prevent 'duplicate' key
            // presses go back to old position
            set_translation(ledge_hang_old_position);

            // update player state
            player_state = PlayerState::DEFAULT;
        }
    }
}

void Player::adapt_rotation() {
    if (camera_mode != CameraMode::DEFAULT) return;

    Vector3 rot = camera->get_rotation_degrees();
    rot.x = 0;
    rot.y = Math::wrapf(rot.y + 180, 0, 360);
    set_rotation_degrees(rot);
}

Vector3 Player::get_movement_dir() {
    Input *input = Input::get_singleton();

    Vector3 move_dir = Vector3::ZERO;
    move_dir.x = input->get_action_strength("player_right") -
                 input->get_action_strength("player_left");
    move_dir.z = input->get_action_strength("player_down") -
                 input->get_action_strength("player_up");
    move_dir =
        move_dir
            .rotated(Vector3::UP, Math::deg2rad(get_rotation_degrees().y + 180))
            .normalized();
    return move_dir;
}

void Player::handle_jump() {
    if (!is_on_floor()) return;

    Input *input = Input::get_singleton();
    if (input->is_action_just_pressed("player_jump")) {
        jump_sound->play();
        velocity.y = jump_force;
        snap = Vector3::ZERO;
    } else if (snap == Vector3::ZERO) {
        snap = Vector3::DOWN;
    }
}

void Player::update_boost() {
    Input *input = Input::get_singleton();

    boost = Math::max<float>(1, boost);
    if (boost == 1 && input->is_action_just_pressed("player_dash")) {
        dash_sound->play();
        boost = boost_force;
    } else if (boost > 1) {
        boost -= boost_force / boost_decay;
    }
}

void Player::handle_glide(float delta) {
    if (is_on_floor() || velocity.y >= 0) return;

    Input *input = Input::get_singleton();
    if (input->is_action_pressed("player_jump")) {
        velocity.y += glide_air_resistance * delta;
    }
}

void Player::set_controllable(bool cont) { controllable = cont; }

void Player::set_id(int64_t id) { this->id = id; }

int64_t Player::get_id() { return id; }

void Player::set_username(String username) { this->username = username; }

String Player::get_username() { return username; }