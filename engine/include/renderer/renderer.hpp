#pragma once

#include "engine/world.hpp"

namespace BlockBinding {
    constexpr uint32_t lightBlock = 0;
    constexpr uint32_t cameraBlock = 1;
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

struct GPUCameraBlock {
    glm::vec3 position {0.0f};
    char padding[4];
    glm::mat4 view {1.0f};
    glm::mat4 projection {1.0f};
};

namespace Renderer {
    void init(World& world);

    GPULightBlock gatherLightData(World& world);
    GPUCameraBlock gatherCameraData(World& world);

    void beginFrame(World& world);
    void renderScene(World& world);
    void endFrame(World& world);
};