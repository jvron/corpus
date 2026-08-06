#include "ecs/components.hpp"
#include "scene/model_spawner.hpp"
#include "resources/resource_manager.hpp"

Entity ModelSpawner::processModelNode(Entity& parentEntity, const ModelAsset& model, const ModelNode& node) {

    Entity nodeEntity = registry.createEntity();

    // nodeEnity is the child of the parentEntity
    Child child = {
        .parent = parentEntity
    };
    registry.insertComponents(nodeEntity, child, node.localTransform, WorldTransform());

    // nodeEntity owns its mesh entities and child node entities.
    Parent parentNode; 

    for (auto& renderer : node.renderers) {

        Child child;
        child.parent = nodeEntity;

        Entity meshEntity =  registry.createEntity();

        Mesh mesh = {
            .handle = renderer.mesh
        };
        Material material = {
            .handle = renderer.material
        };

        registry.insertComponents(meshEntity, material, mesh, child, Transform(), WorldTransform(), Renderable()); 

        parentNode.children[parentNode.count++] = meshEntity;
    }

    for (ModelNodeID childNodeID : node.children) {

        const ModelNode& childNode = model.nodes[childNodeID];

        Entity childEntity = processModelNode(nodeEntity, model, childNode);
        parentNode.children[parentNode.count++] = childEntity;
    }

    registry.insert(nodeEntity, parentNode);

    return nodeEntity;
}

Entity ModelSpawner::instantiate(ModelHandle modelHandle) {

    Entity root = registry.createEntity();

    const ModelAsset& modelAsset = resourceManager.getModelAsset(modelHandle);

    Entity model = processModelNode(root, modelAsset, modelAsset.nodes[modelAsset.root]);

    Parent parent;
    parent.children[parent.count++] = model;
    registry.insertComponents(root, parent, Transform(), WorldTransform());

    return root;
}