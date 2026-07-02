#pragma once

#include "engine/world.hpp"

namespace BlockBinding {
    constexpr uint32_t lightBlock = 0;
}

struct GPUPointLight {
    glm::vec4 color {1.0f};
    glm::vec4 position {0.0f};
};

struct GPULightBlock {
    int lightCount {};
    char padding[12];
    GPUPointLight pointLight[32];
};

namespace Renderer {
    void init(World& world);

    GPULightBlock gatherLightData(World& world);

    void beginFrame(World& world);
    void renderScene(World& world);
    void endFrame(World& world);
};