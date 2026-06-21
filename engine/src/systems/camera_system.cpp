#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "ecs/components.hpp"
#include "engine/systems.hpp"


void CameraSystem::updateView(Camera& camera, glm::mat4& view) {

    view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
}

void CameraSystem::updateProjection(World& world, Camera& camera, glm::mat4& projection) {
    
   projection = glm::perspective(glm::radians(camera.fov), world.engineState.windowState.aspect, camera.nearPlane, camera.farPlane);
}

