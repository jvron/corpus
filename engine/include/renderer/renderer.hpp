#pragma once

#include <glm/ext/matrix_float4x4.hpp>

#include "ecs/components.hpp"
#include "engine/world.hpp"
#include "resources/resource_manager.hpp"

constexpr int MaxPointLights = 32;
constexpr int MaxSpotlights = 12;
constexpr int MaxDirectionalLights = 1;

namespace TextureUnit {
    constexpr uint32_t diffuseMap = 0;
    constexpr uint32_t specularMap = 1;
}

namespace BlockBinding {
    constexpr uint32_t lightBlock = 0;
    constexpr uint32_t cameraBlock = 1;
}

struct GPUPointLight {
    glm::vec4 color {};
    glm::vec4 position {};

    float intensity {};
    float radius {};
    char padding1[8];
};

struct GPUDirectionalLight {
    glm::vec4 color {};
    glm::vec4 direction {};

    float ambientStrength {};
    float intensity {};
    char padding[8];
};

struct GPUSpotlight {
    glm::vec4 color {};
    glm::vec4 direction {};
    glm::vec4 position {};

    float intensity {};
    float radius {};
    float innerCutOff {};
    float outerCutOff {};
};

struct GPULightBlock {

    GPUDirectionalLight dirLight;

    int pointLightCount {};
    char padding1[12];
    GPUPointLight pointLights[MaxPointLights];

    int spotlightCount {};
    char padding2[12];
    GPUSpotlight spotlights[MaxSpotlights];
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

    glm::mat4 getModelMatrix(const Transform& transform);

    void drawMesh(World& world);
    void drawModel(World& world);

    void beginFrame(World& world);
    void renderScene(World& world);
    void endFrame(World& world);
};