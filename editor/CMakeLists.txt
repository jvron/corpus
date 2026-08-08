cmake_minimum_required(VERSION 3.20)

add_executable(corpus
    src/main.cpp
    src/editor.cpp
    src/imgui_layer.cpp
    src/editor_actions.cpp
    src/panels/hierarchy_panel.cpp
    src/panels/inspector_panel.cpp
)

add_library(ImGui STATIC
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp

    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp

    third_party/imgui/misc/cpp/imgui_stdlib.cpp
)

target_include_directories(ImGui PUBLIC
    third_party/imgui
    third_party/imgui/backends
    third_party/imgui/misc/cpp
)

target_link_libraries(ImGui PUBLIC glfw)

# Editor dependencies

target_link_libraries(corpus PRIVATE
    engine
    ImGui
    glfw
)

target_include_directories(corpus PRIVATE
    include
)

target_compile_features(corpus PRIVATE cxx_std_20)