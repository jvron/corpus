#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

#include "ecs/components.hpp"
#include "engine/world.hpp"

namespace CameraControlSystem {

    static const glm::vec3 worldUp {0.0f, 1.0f, 0.0f};

    void update(World& world);

    void moveForward(Camera& camera, CameraController& ctrl, float dt);
    void moveBackwards(Camera& camera, CameraController& ctrl, float dt);
    void moveRight(Camera& camera, CameraController& ctrl, float dt);
    void moveLeft(Camera& camera, CameraController& ctrl, float dt);
}

namespace CameraSystem {

    void updateView(Camera& camera, glm::mat4& view);
    void updateProjection(World& world, Camera& camera, glm::mat4& projection);
}
