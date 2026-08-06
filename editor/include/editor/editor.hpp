#pragma once

#include "ecs/components.hpp"
#include "engine/world.hpp"
#include "engine/engine.hpp"

struct EditorState {
    Entity selectedEntity;
};

class Editor {

public:
    void init(World& world);
    void run(World& world, Engine& engine);
    void shutdown();

private:
    EditorState state;
};