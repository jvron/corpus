#pragma once

struct World;

namespace ImGuiLayer {

    void init(World& world);

    void beginFrame();
    void render();

    void shutdown();
}