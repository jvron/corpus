#pragma once

#include "ecs/components.hpp"
#include "ecs/registry.hpp"
#include "resources/resource_manager.hpp"

class ModelSpawner {

public:
    ModelSpawner(Registry& registry, ResourceManager& resourceManager) : 
        registry(registry), resourceManager(resourceManager) {} 

    Entity instantiate(ModelHandle modelHandle);

private:
    Registry& registry;
    ResourceManager& resourceManager;

    Entity processModelNode(Entity& parent, const ModelAsset& model, const ModelNode& node);
};