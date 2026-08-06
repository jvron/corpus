#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "editor/imgui_layer.hpp"
#include "engine/world.hpp"

void ImGuiLayer::init(World& world) {

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(world.engineState.windowState.handle, false);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void ImGuiLayer::shutdown() {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(); 
}