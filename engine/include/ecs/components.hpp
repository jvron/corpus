#pragma once 

#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstdint>

using MeshHandle = uint32_t;
using ShaderHandle = uint32_t;
using EntityID = uint32_t;

struct Entity {
    EntityID id {};
    uint32_t generation {};
};

struct Color {
    glm::vec4 value {};
};

struct Transform {
    glm::vec3 position {0.0f, 0.0f, 0.0f};
    glm::vec3 scale {1.0f, 1.0f, 1.0f};
    glm::vec3 rotation {0.0f, 0.0f, 0.0f};  
};

struct Mesh {  
    MeshHandle meshHandle {};
};

struct Renderable {
    bool visible {true};
};

struct Material {
    ShaderHandle shaderHandle {};
    glm::vec4 baseColor {1.0f, 1.0f, 1.0f, 1.0f};
};
