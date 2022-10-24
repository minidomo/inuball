#include "task_list.h"

#include <InputEventKey.hpp>
#include <GlobalConstants.hpp>
#include <ResourceLoader.hpp>
#include <MarginContainer.hpp>
#include <Label.hpp>
#include <string>

void TaskList::_register_methods() {
    register_method("_ready", &TaskList::_ready);
    register_method("_physics_process", &TaskList::_physics_process);
    register_method("_process", &TaskList::_process);
    register_method("_input", &TaskList::_input);
    register_method("add_task", &TaskList::add_task);
    register_method("remove_task", &TaskList::remove_task);
}

void TaskList::_init() {
    next_task_idx = 1;
    last_task_idx = 1;

    ResourceLoader* reLo = ResourceLoader::get_singleton();
    taskRef = reLo->load("res://scenes/gui/Task.tscn");
}

void TaskList::_ready() {
}

void TaskList::_process(float delta) {
}

void TaskList::_physics_process(float delta) {
}

void TaskList::_input(Ref<InputEvent> event) {
    InputEventKey *key_event =
        Object::cast_to<InputEventKey>(event.ptr());
    if (key_event) {
        int64_t key = key_event->get_scancode();
        if (key == GlobalConstants::KEY_R && key_event->is_pressed()) {
            Godot::print("adding task");
            add_task(next_task_idx);
            next_task_idx++;
        }
        else if (key == GlobalConstants::KEY_T && key_event->is_pressed()) {
            Godot::print("removing task");
            remove_task(last_task_idx);
            last_task_idx++;
        }
    }
}

void TaskList::add_task(uint32_t task_num) {
    auto task = taskRef->instance();
    String task_str = "task_" + String(std::to_string(task_num).c_str());
    task->set_name(task_str);
    Label* task_label = Object::cast_to<Label>(task->get_node("TaskDesc"));
    task_label->set_text(task_str);
    add_child(task);
}

void TaskList::remove_task(uint32_t task_num) {
    NodePath* task_str = new NodePath("task_" + String(std::to_string(task_num).c_str()));
    auto task = get_node(*task_str);
    if (task) {
        task->queue_free();
    }
}