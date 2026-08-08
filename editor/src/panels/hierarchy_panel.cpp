#include <imgui.h>
#include <optional>
#include <string>

#include "core/string_pool.hpp"
#include "ecs/components.hpp"
#include "editor/editor.hpp"
#include "editor/editor_actions.hpp"
#include "engine/world.hpp"

#include "editor/panels.hpp"

void HierarchyPanel::draw(World& world, EditorState& state) {

    ImGui::Begin("Hierarchy");

    if (ImGui::Button("Add")) {

        Entity newEntity = EditorActions::createEntity(world, "New Entity");

        state.selectEntity(newEntity);
        state.updateNameBuffer(world);
    }

    for (Entity entity : world.registry.entities()) {

        std::string label;

        if (world.registry.hasComponent<Name>(entity)) {

            StringID id = world.registry.getComponent<Name>(entity).id;
            label = world.stringPool.getString(id);

            if (label.empty()) {
                label = "<Unnamed>";
            }
        }
        else {
            label = "Entity_" + std::to_string(entity.id);
        }

        label = label + "##" + std::to_string(entity.id);

        bool selected = state.isSelected(entity);

        if (ImGui::Selectable(label.c_str(), selected)) {

            state.selectEntity(entity);
            state.updateNameBuffer(world);
        }
        
        if (ImGui::BeginPopupContextItem()) {

            if (ImGui::MenuItem("Delete")) {

                EditorActions::deleteEntity(world, entity);

                if (state.isSelected(entity)) {
                    state.clearSelection();
                }
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}