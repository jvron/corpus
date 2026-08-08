#include <imgui.h>
#include <cstring>
#include <string_view>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>

#include "core/string_pool.hpp"
#include "ecs/components.hpp"
#include "engine/world.hpp"

#include "editor/editor.hpp"
#include "editor/panels.hpp"

void InspectorPanel::drawName(World& world, EditorState& state) {

    Entity entity = *state.selectedEntity;

    if (world.registry.hasComponent<Name>(entity)) {

        Name& name = world.registry.getComponent<Name>(entity);

        std::string_view name_str = world.stringPool.getString(name.id);

        if (ImGui::InputText("Name", state.nameBuffer.data(), MaxNameLength)) {

            StringID newNameId = world.stringPool.intern(state.nameBuffer.data());

            name.id = newNameId;
        }
    }
}

void InspectorPanel::drawTransform(World& world, EditorState& state) {

    Entity entity = *state.selectedEntity;

    if (world.registry.hasComponent<Transform>(entity)) {

        Transform& transform = world.registry.getComponent<Transform>(entity);

        if (ImGui::CollapsingHeader("Transform")) {

            glm::vec3 rotationDegrees = glm::degrees(transform.rotation);

            ImGui::DragFloat3("Position", &transform.position.x);
            ImGui::DragFloat3("Rotation", &rotationDegrees.x);
            ImGui::DragFloat3("Scale", &transform.scale.x);
        }
    }
}

void InspectorPanel::drawRenderable(World& world, EditorState& state) {

    Entity entity = *state.selectedEntity;

    if (world.registry.hasComponent<Renderable>(entity)) {

        Renderable& renderable = world.registry.getComponent<Renderable>(entity);

        if (ImGui::CollapsingHeader("Renderable")) {

            ImGui::Checkbox("Visible", &renderable.visible);
        }
    }
}

void InspectorPanel::draw(World& world, EditorState& state) {

    ImGui::Begin("Inspector");

    if (!state.selectedEntity.has_value()) {

        ImGui::Text("No entity selected");
        ImGui::End();
        return;
    }

    Entity entity = *state.selectedEntity; 

    ImGui::Text("Entity ID: %u", entity.id);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    drawName(world, state);
    drawTransform(world, state);
    drawRenderable(world, state);

    ImGui::End();
}