#pragma once

#include "ecs/components.hpp"
#include "engine/world.hpp"
#include "engine/engine.hpp"

constexpr size_t MaxNameLength = 128;

struct EditorState {

    std::optional<Entity> selectedEntity;
    std::array<char, MaxNameLength> nameBuffer {};

    void selectEntity(Entity entity);
    void clearSelection();
    void updateNameBuffer(World& world);
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