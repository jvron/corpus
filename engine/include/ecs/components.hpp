#pragma once 
 
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstdint>
#include <vector>
#include <string>

using MeshHandle = uint32_t;
using ShaderHandle = uint32_t;
using MaterialHandle = uint32_t;
using TextureHandle = uint32_t;
using EntityID = uint32_t;

struct Entity {
    EntityID id {};
    uint32_t generation {};
};

struct Parent {
    std::vector<Entity> children;
};

struct Child {
    Entity parent;
};

struct Color {
    glm::vec4 value {};
};

struct Transform {
    glm::vec3 position {0.0f};
    glm::vec3 scale {1.0f};
    glm::vec3 rotation {0.0f};  
};

struct Mesh {  
    MeshHandle handle {};
};

struct Material {
    MaterialHandle handle;
};

struct Renderable {
    bool visible {true};
};

struct Camera {
    float fov {45.0f};
    float nearPlane {0.1f};
    float farPlane {100.0f};

    glm::vec3 position {0.0f};
    glm::vec3 front {0.0f, 0.0f, -1.0f}; 
    glm::vec3 up {0.0f, 1.0f, 0.0f};
    glm::vec3 right {1.0f, 0.0f, 0.0f};
};

struct CameraController {
    float speed {0.5f};
    float yaw {};
    float pitch {};

    float sensitivityMultiplier {1.0f};

    bool enableMouseLook {true};
    bool enableMovement {true};
};

struct PointLight {
    glm::vec3 color {1.0f};
    glm::vec3 position {0.0f};

    float intensity {1.0f};
    float radius {};
};

struct DirectionalLight {
    glm::vec3 color {1.0f};
    glm::vec3 direction {0.0f};
    
    float ambientStrength {1.0f};
    float intensity {1.0f};
};

struct Spotlight {
    glm::vec3 color {1.0f};
    glm::vec3 position {0.0f};
    glm::vec3 direction {0.0f};

    float intensity {1.0f};
    float radius {};
    float innerCutOff {};
    float outerCutOff {};
};