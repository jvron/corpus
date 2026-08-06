#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "ecs/components.hpp"
#include "ecs/registry.hpp"
#include "engine/systems.hpp"
#include "engine/world.hpp"
#include "ecs/view.hpp"

glm::mat4 TransformSystem::getLocalMatrix(const Transform& transform) {

    glm::mat4 localMatrix = glm::mat4(1.0f);
    localMatrix = glm::translate(localMatrix, transform.position);
    localMatrix = glm::rotate(localMatrix, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    localMatrix = glm::rotate(localMatrix, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    localMatrix = glm::rotate(localMatrix, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    localMatrix = glm::scale(localMatrix, transform.scale);

    //localMatrix = T * R * S 

    return localMatrix;
}

void TransformSystem::updateWorldMatrix(Entity entity, const glm::mat4& parentWorld, Registry& registry) {

    Transform& localTransform = registry.getComponent<Transform>(entity);
    glm::mat4 localMatrix = getLocalMatrix(localTransform);

    WorldTransform& worldTransform = registry.getComponent<WorldTransform>(entity);

    glm::mat4 thisWorld = parentWorld * localMatrix;

    worldTransform.matrix = thisWorld;

    if (registry.hasComponent<Parent>(entity)) {

        Parent& parent = registry.getComponent<Parent>(entity);

        for (const Entity child_entity : parent.children) {

            updateWorldMatrix(child_entity, thisWorld, registry);
        }
    }
}

void TransformSystem::update(World& world) {

    Registry& registry = world.registry;

    glm::mat4 identity {1.0f};

    for (auto [entity, worldTransform, localTransform] : View<WorldTransform, Transform>(registry)) {
        
        // update only root entities 
        if (!registry.hasComponent<Child>(entity)) {

            updateWorldMatrix(entity, identity, registry);
        }
    }
}