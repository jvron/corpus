#include <glm/trigonometric.hpp>
#include <iostream>

#include "ecs/components.hpp"
#include "engine/systems.hpp"
#include "engine/world.hpp"
#include "glm/ext/vector_float3.hpp"

void CameraControlSystem::update(World& world) {

    CameraController& ctrl = world.registry.getComponent<CameraController>(world.activeCamera);

    if (!ctrl.enableMouseLook) {
        return;
    }
    
    Camera& camera = world.registry.getComponent<Camera>(world.activeCamera);

    InputState& inputState = world.engineState.inputState;

    ctrl.yaw += inputState.mouseDeltaX * world.engineConfig.inputConfig.sensitivity * ctrl.sensitivityMultiplier;
    ctrl.pitch += inputState.mouseDeltaY * world.engineConfig.inputConfig.sensitivity * ctrl.sensitivityMultiplier;
    
    if (ctrl.pitch > 89.0f) {
        ctrl.pitch = 89.0f;
    }
    if (ctrl.pitch < -89.0f) {
        ctrl.pitch = -89.0f;
    }
    
    inputState.mouseDeltaX = 0.0f;
    inputState.mouseDeltaY = 0.0f;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(ctrl.yaw)) * cos(glm::radians(ctrl.pitch));
    direction.y = sin(glm::radians(ctrl.pitch));
    direction.z = sin(glm::radians(ctrl.yaw)) * cos(glm::radians(ctrl.pitch));
    camera.front = glm::normalize(direction);

    camera.right = glm::normalize(glm::cross(camera.front, worldUp));
    camera.up = glm::normalize(glm::cross(camera.right, camera.front));
}

void CameraControlSystem::moveForward(Camera& camera, CameraController& ctrl , float dt) {

    if (!ctrl.enableMovement) {
        return;
    }
    camera.position += camera.front * ctrl.speed * dt;
}

void CameraControlSystem::moveBackwards(Camera& camera, CameraController& ctrl, float dt) {
    
    if (!ctrl.enableMovement) {
        return;
    }
    camera.position -= camera.front * ctrl.speed * dt;
}

void CameraControlSystem::moveRight(Camera& camera, CameraController& ctrl, float dt) {
    
    if (!ctrl.enableMovement) {
        return;
    }
    camera.position += camera.right * ctrl.speed * dt;
}

void CameraControlSystem::moveLeft(Camera& camera,  CameraController& ctrl, float dt) {
    
    if (!ctrl.enableMovement) {
        return;
    }
    camera.position -= camera.right * ctrl.speed * dt;
}