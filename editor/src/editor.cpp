#include "editor/editor.hpp"
#include "editor/imgui_layer.hpp"
#include "engine/engine.hpp"
#include "engine/world.hpp"
#include "platform/window.hpp"

void Editor::init(World& world) {

    ImGuiLayer::init(world);
}

void Editor::run(World& world, Engine& engine) {

    while (!Window::shouldClose(world)) {

        engine.runFrame(world);   
    }
}

void Editor::shutdown() {

  ImGuiLayer::shutdown();
}