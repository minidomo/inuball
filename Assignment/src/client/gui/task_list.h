#ifndef A2_TASK_LIST_H
#define A2_TASK_LIST_H

#include <InputEvent.hpp>
#include <PackedScene.hpp>
#include <VBoxContainer.hpp>

#include "../common.h"

class TaskList : public VBoxContainer {
    GODOT_CLASS(TaskList, VBoxContainer);

   private:
    uint32_t next_task_idx;
    uint32_t last_task_idx;
    Ref<PackedScene> taskRef;

   public:
    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
    void _input(Ref<InputEvent> event);

   private:
    void add_task(uint32_t task_num);
    void remove_task(uint32_t task_num);
};

#endif