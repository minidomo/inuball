#include "timer.h"

#include <Button.hpp>
#include <GlobalConstants.hpp>
#include <Input.hpp>
#include <InputEventKey.hpp>
#include <Label.hpp>
#include <ResourceLoader.hpp>
#include <memory>
#include <string>

#include "../globals.h"
#include "../objects/player.h"
#include "../singletons/client.h"
#include "../util.h"

void GUITimer::_register_methods() {
    register_method("_ready", &GUITimer::_ready);
    register_method("tick", &GUITimer::tick);
    register_method("exit_game", &GUITimer::exit_game);
    register_method("play_again", &GUITimer::play_again);
    register_property<GUITimer, int>("game_length", &GUITimer::game_length, 60);
}

void GUITimer::_init() { game_length = 60; }

void GUITimer::_ready() {
    Dictionary lobby_data = Client::get_singleton(this)->get_lobby_data();

    if (GLOBALS::instance.multiplayer) {
        time = lobby_data["game_length"];
    } else {
        time = game_length;
    }
    GLOBALS::instance.timer = this;

    time_label = Object::cast_to<Label>(get_node("Time"));
    time_label->set_text(Util::get_time_str(time));

    this->get_node("Time/Timer")->connect("timeout", this, "tick");
}

void GUITimer::tick() {
    if (this->time-- > 0) {
        time_label->set_text(Util::get_time_str(time));
    } else {
        // stop the tick
        get_node("Time/Timer")->disconnect("timeout", this, "tick");

        if (GLOBALS::instance.multiplayer) {
            Client::get_singleton(this)->request_game_end();
        }

        Input *input = Input::get_singleton();
        input->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);

        // Player *player = Object::cast_to<Player>(GLOBALS::instance.player);
        // player->set_controllable(false);

        auto gameover =
            Object::cast_to<CanvasItem>(this->get_node("/root/Main/GameOver"));
        auto scoring = gameover->get_node("background/MarginContainer/Scoring");
        auto winner = Object::cast_to<Label>(scoring->get_node("Winner"));
        auto menu = Object::cast_to<Button>(scoring->get_node("Buttons/Menu"));

        if (GLOBALS::instance.multiplayer) {
            menu->set_text("Return to Lobby");
        } else {
            menu->set_text("Return to Menu");
        }

        menu->connect("pressed", this, "exit_game");

        auto stats = GLOBALS::instance.stats_container;
        auto red_score = Object::cast_to<Label>(stats->get_node("ScoreRed"))
                             ->get_text()
                             .to_int();
        auto blue_score = Object::cast_to<Label>(stats->get_node("ScoreBlue"))
                              ->get_text()
                              .to_int();

        char buffer[64];

        if (!GLOBALS::instance.multiplayer) {
            auto play = Object::cast_to<Button>(menu->duplicate());
            play->set_text("Play Again");
            menu->get_parent()->add_child(play);
            play->connect("pressed", this, "play_again");
            snprintf(buffer, 64, "Score: %lld\n", red_score + blue_score);
        } else {
            if (red_score > blue_score)
                snprintf(buffer, 64, "Winner: Red Team!\n");
            if (red_score < blue_score)
                snprintf(buffer, 64, "Winner: Blue Team!\n");
            if (red_score == blue_score) snprintf(buffer, 64, "Tie Game!");
        }
        winner->set_text(buffer);

        auto score_listing = scoring->get_node("ScoreListing");
        auto listing = scoring->get_node("ScoreListing");
        auto red_stat = Object::cast_to<Label>(winner->duplicate());
        auto blue_stat = Object::cast_to<Label>(winner->duplicate());

        snprintf(buffer, 64, "Red: %lld\n", red_score);
        red_stat->set_text(buffer);
        snprintf(buffer, 64, "Blue: %lld\n", blue_score);
        blue_stat->set_text(buffer);

        gameover->set_visible(true);
    }
}

void GUITimer::exit_game() {
    if (GLOBALS::instance.multiplayer) {
        Client::get_singleton(this)->request_return_to_lobby();
    } else {
        get_tree()->change_scene("res://scenes/gui/MainMenu.tscn");
    }
}

void GUITimer::play_again() {
    get_tree()->change_scene(
        "res://scenes/playground/CustomWorldPlayground.tscn");
}

void GUITimer::set_time(int16_t time) { this->time = time; }

int16_t GUITimer::get_time() { return time; }