#include "scene/scene_manager.hpp"
#include "ecs/components.hpp"
#include "resources/resource_manager.hpp"

Entity SceneManager::processSceneNode(Entity& parentEntity, const SceneAsset& scene, const SceneNode& node) {

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

        parentNode.children.push_back(meshEntity);
    }

    for (SceneNodeID childNodeID : node.children) {

        const SceneNode& childNode = scene.nodes[childNodeID];

        Entity childEntity = processSceneNode(nodeEntity, scene, childNode);
        parentNode.children.push_back(childEntity);
    }

    registry.insert(nodeEntity, parentNode);

    return nodeEntity;
}

Entity SceneManager::instantiate(SceneHandle sceneHandle) {

    Entity root = registry.createEntity();

    const SceneAsset& sceneAsset = resourceManager.getSceneAsset(sceneHandle);

    Entity scene = processSceneNode(root, sceneAsset, sceneAsset.nodes[sceneAsset.root]);

    Parent parent;
    parent.children.push_back(scene);
    registry.insertComponents(root, parent, Transform(), WorldTransform());

    return root;
}