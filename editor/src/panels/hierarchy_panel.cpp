#include <imgui.h>
#include <string>

#include "core/string_pool.hpp"
#include "ecs/components.hpp"
#include "editor/editor.hpp"
#include "engine/world.hpp"

#include "editor/panels.hpp"

void HierarchyPanel::draw(World& world, EditorState& state) {

    ImGui::Begin("Hierarchy");

    for (Entity entity : world.registry.entities()) {

        std::string name;

        if (world.registry.hasComponent<Name>(entity)) {

            StringID id = world.registry.getComponent<Name>(entity).id;
            name = world.stringPool.getString(id);

            if (name.empty()) {
                name = "<Unnamed>";
            }
        }
        else {
            name = "Entity_" + std::to_string(entity.id);
        }

        bool selected = state.isSelected(entity);

        if (ImGui::Selectable(name.c_str(), selected)) {

            state.selectEntity(entity);
            state.updateNameBuffer(world);
        }  
    }

    ImGui::End();
}