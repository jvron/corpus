#pragma once

struct EditorState;
struct World;

namespace HierarchyPanel {

    void draw(World& world, EditorState& state);
}

namespace InspectorPanel {

    void draw(World& world, EditorState& state);
}