#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "ecs/components.hpp"
#include "ecs/registry.hpp"
#include "engine/engine.hpp"
#include "platform/input.hpp"
#include "resources/resource_manager.hpp"

#include "header.hpp"

struct TestCube {};


int main()
{
    World world;

    EngineConfig& config = world.engineConfig;

    config.windowConfig.width = 1000;
    config.windowConfig.height = 800;
    config.windowConfig.title = "Corpus Sandbox";
    config.inputConfig.sensitivity = 0.1f;

    world.engineState.renderState.clearColor.value = {
        0.30f,
        0.30f,
        0.32f,
        1.0f
    };

    Registry& registry = world.registry;

    registry.registerComponent<TestCube>();

    Engine engine;
    engine.startUp(world);


    Camera camera {
        .fov = 80.0f,
        .nearPlane = 0.1f,
        .farPlane = 500.0f
    };

    CameraController controller {
        .speed = 5.0f,
        .sensitivityMultiplier = 2.5f
    };

    Entity cameraEntity = registry.createEntity();

    registry.insert(cameraEntity, camera);
    registry.insert(cameraEntity, controller);

    world.activeCamera = cameraEntity;

    Input::disableCursor(world);


    Renderable cubeRenderable;

    Mesh cubeMesh;
    loadcubeMesh(world, cubeMesh);


    MaterialAsset materialAsset;

    loadShader(world, materialAsset);

    materialAsset.baseColor = {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    };

    materialAsset.specularStrength = 0.5f;
    materialAsset.shininess = 32.0f;

    loadTexture(world, materialAsset);


    Material material = world.resourceManager.loadMaterial(materialAsset);


    MaterialAsset lightMaterialAsset;

    loadLightShader(world, lightMaterialAsset);

    Material lightMaterial =
        world.resourceManager.loadMaterial(lightMaterialAsset);

    Tex2DParameters textureParams;

    textureParams.magFilter = TexFilter::Nearest;
    textureParams.minFilter = TexFilter::NearestMipmapNearest;
    textureParams.enableMipmap = true;
    textureParams.wrapS = TexWrap::Repeat;
    textureParams.wrapT = TexWrap::Repeat;


    ModelOptions options;

    options.diffuseParameters = textureParams;
    options.specularParameters = textureParams;
    options.shader = materialAsset.shaderHandle;
    options.materialSpecularStrength = 0.5f;
    options.materialShininess = 32.0f;


    Entity modelEntity = registry.createEntity();

    Model model =
        world.resourceManager.loadModel(
            "examples/sandbox/assets/models/backpack/backpack.obj",
            options
        );

    Transform modelTransform;

    modelTransform.position = {
        1.0f,
        1.0f,
        0.0f
    };

    registry.insertComponents(
        modelEntity,
        model,
        modelTransform,
        cubeRenderable
    );

    for (int i = 0; i < 2; i++)
    {
        Entity cube = registry.createEntity();

        registry.insert(cube, TestCube{});

        Transform transform {
            .position = {4.0f, 0.0f, 0.0f},
            .scale = {1.0f, 1.0f, 1.0f}
        };

        registry.insert(cube, transform);

        registry.insertComponents(
            cube,
            cubeMesh,
            material,
            cubeRenderable
        );
    }

    PointLight pointLight;

    pointLight.radius = 62.0f;
    pointLight.intensity = 20.0f;
    pointLight.position = {4.0f, 0.0f, 0.0f};

    Entity lightEntity = registry.createEntity();

    Transform lightTransform {
        .position = {10.0f, 0.0f, 0.0f},
        .scale = {0.5f, 0.5f, 0.5f}
    };

    registry.insert(lightEntity, lightTransform);

    registry.insertComponents(
        lightEntity,
        cubeMesh,
        cubeRenderable,
        lightMaterial,
          pointLight
    );

    DirectionalLight directionalLight;

    directionalLight.ambientStrength = 0.4f;
    directionalLight.direction = {0.0f, -1.0f, 0.0f};
    directionalLight.intensity = 10.0f;

    Entity directionalLightEntity = registry.createEntity();

    registry.insert(
        directionalLightEntity,
        directionalLight
    );

    Spotlight spotlight;

    spotlight.innerCutOff = 12.5f;
    spotlight.outerCutOff = 24.5f;
    spotlight.intensity = 10.0f;
    spotlight.radius = 52.0f;
    spotlight.direction = {0.0f, -1.0f, 0.0f};
    spotlight.position = {0.5f, 0.5f, -1.0f};


    Entity spotlightEntity = registry.createEntity();

    registry.insert(
        spotlightEntity,
        spotlight
    );

    //engine.scheduler.addSystem(Stage::Update, testUniform);
    engine.scheduler.addSystem(Stage::Input, testCamera);
    engine.scheduler.addSystem(Stage::Update, testTransform);

    engine.run(world);

    engine.shutDown(world);

    return 0;
}