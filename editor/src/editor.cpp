#include <imgui.h>

#include "ecs/components.hpp"
#include "engine/engine.hpp"
#include "engine/world.hpp"
#include "platform/window.hpp"

#include "editor/editor.hpp"
#include "editor/imgui_layer.hpp"
#include "editor/panels.hpp"

bool EditorState::isSelected(Entity entity) const {

    if (!selectedEntity.has_value()) {
        return false;
    }

    return selectedEntity.value() == entity; 
}

void Editor::init(World& world) {

    ImGuiLayer::init(world);
}

void Editor::draw(World& world) {

    HierarchyPanel::draw(world, state);
    InspectorPanel::draw(world, state);
}

void Editor::run(World& world, Engine& engine) {

    while (!Window::shouldClose(world)) {

        engine.scheduler.runStage(Stage::Begin);
        engine.scheduler.runStage(Stage::Input);
        engine.scheduler.runStage(Stage::Update);

        ImGuiLayer::beginFrame();

        engine.scheduler.runStage(Stage::Render);
    
        draw(world);
    
        ImGuiLayer::render();

        engine.scheduler.runStage(Stage::End);
    }
}

void Editor::shutdown() {

  ImGuiLayer::shutdown();
}