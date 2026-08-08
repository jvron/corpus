#pragma once

#include "ecs/components.hpp"
#include "engine/world.hpp"
#include "engine/engine.hpp"

struct EditorState {

    std::optional<Entity> selectedEntity;

    bool isSelected(Entity entity) const;
};

class Editor {

public:
    void init(World& world);
    void run(World& world, Engine& engine);
    void shutdown();

private:
    EditorState state;

    void draw(World& world);
};