#include "actions/action.h"
#include "actions/sheep/attack_action.h"
#include "actions/sheep/base_sheep_action.h"
#include "actions/sheep/flee_action.h"
#include "actions/sheep/wander_action.h"
#include "common.h"
#include "gui/connecting_screen.h"
#include "gui/main_menu.h"
#include "gui/multiplayer/multiplayer_lobby.h"
#include "gui/multiplayer/multiplayer_lobby_card.h"
#include "gui/multiplayer/multiplayer_lobby_create.h"
#include "gui/multiplayer/multiplayer_lobby_create_setting.h"
#include "gui/multiplayer/multiplayer_lobby_player.h"
#include "gui/multiplayer/multiplayer_main_menu.h"
#include "gui/task_list.h"
#include "gui/timer.h"
#include "objects/animals/animal.h"
#include "objects/camera.h"
#include "objects/finite_state_machine.h"
#include "objects/goal.h"
#include "objects/ledge_detector.h"
#include "objects/player.h"
#include "signals/looking_at_receiver.h"
#include "singletons/client.h"
#include "sound/background_music.h"
#include "world_gen/chunk.h"
#include "world_gen/custom_world.h"
#include "world_gen/kill_floor.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT
godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    Godot::nativescript_init(handle);

    register_class<Camera>();
    register_class<Player>();
    register_class<GUITimer>();
    register_class<TaskList>();
    register_class<LedgeDetector>();
    register_class<CustomWorld>();
    register_class<Chunk>();
    register_class<KillFloor>();
    register_class<BackgroundMusic>();
    register_class<Goal>();

    register_class<Chicken>();
    register_class<Sheep>();

    register_class<ConnectingScreen>();
    register_class<MainMenu>();

    register_class<MultiplayerMainMenu>();
    register_class<MultiplayerLobbyCard>();
    register_class<MultiplayerLobbyCreate>();
    register_class<MultiplayerLobbyCreateSetting>();
    register_class<MultiplayerLobby>();
    register_class<MultiplayerLobbyPlayer>();

    register_class<Client>();

    register_class<FiniteStateMachine>();
    register_class<Action>();
    register_class<BaseSheepAction>();
    register_class<AttackAction>();
    register_class<FleeAction>();
    register_class<WanderAction>();
}
