#include <imgui.h>

#include "ecs/components.hpp"
#include "engine/world.hpp"

#include "editor/editor.hpp"
#include "editor/panels.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"

void InspectorPanel::draw(World& world, EditorState& state) {

    ImGui::Begin("Inspector");

    if (!state.selectedEntity.has_value()) {

        ImGui::Text("No entity selected");
        ImGui::End();
        return;
    }

    Entity entity = state.selectedEntity.value(); 

    ImGui::Text("Entity ID: %u", entity.id);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (world.registry.hasComponent<Transform>(entity)) {

        Transform& transform = world.registry.getComponent<Transform>(entity);

        if (ImGui::CollapsingHeader("Transform")) {

            glm::vec3 rotationDegrees = glm::degrees(transform.rotation);

            ImGui::DragFloat3("Position", &transform.position.x);
            ImGui::DragFloat3("Rotation", &rotationDegrees.x);
            ImGui::DragFloat3("Scale", &transform.scale.x);
        }
    }

    if (world.registry.hasComponent<Renderable>(entity)) {

        Renderable& renderable = world.registry.getComponent<Renderable>(entity);

        if (ImGui::CollapsingHeader("Renderable")) {

            ImGui::Checkbox("Visible", &renderable.visible);
        }
    }

    ImGui::End();
}