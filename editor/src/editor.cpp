#include <cstddef>
#include <cstring>
#include <imgui.h>
#include <string_view>

#include "core/string_pool.hpp"
#include "ecs/components.hpp"
#include "engine/engine.hpp"
#include "engine/world.hpp"
#include "platform/window.hpp"

#include "editor/editor.hpp"
#include "editor/imgui_layer.hpp"
#include "editor/panels.hpp"

void EditorState::selectEntity(Entity entity)  {
    selectedEntity = entity;
}

void EditorState::updateNameBuffer(World& world)  {

    if (!selectedEntity.has_value()) {
        return;
    }

    Entity entity = *selectedEntity;

    if (!world.registry.hasComponent<Name>(entity)) {
        return;
    }

    Name& name = world.registry.getComponent<Name>(entity);

    std::string_view nameStr = world.stringPool.getString(name.id);

    size_t length = std::min(nameStr.length(), nameBuffer.size() - 1);

    std::memcpy(nameBuffer.data(), nameStr.data(), length);

    nameBuffer[length] = '\0';
}

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