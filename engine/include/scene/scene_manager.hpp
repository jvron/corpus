#pragma once

#include "ecs/components.hpp"
#include "ecs/registry.hpp"
#include "resources/resource_manager.hpp"

class SceneManager {

public:
    SceneManager(Registry& registry, ResourceManager& resourceManager) : 
        registry(registry), resourceManager(resourceManager) {} 

    Entity instantiate(SceneHandle sceneHandle);

private:
    Registry& registry;
    ResourceManager& resourceManager;

    Entity processSceneNode(Entity& parent, const SceneAsset& scene, const SceneNode& node);
};