#pragma once

#include "editor.hpp"

struct EditorState;
struct World;

namespace HierarchyPanel {

    void draw(World& world, EditorState& state);
}

namespace InspectorPanel {

    void drawName(World& world, EditorState& state);
    void drawTransform(World& world, EditorState& state);
    void drawRenderable(World& world, EditorState& state);
    void draw(World& world, EditorState& state);
}