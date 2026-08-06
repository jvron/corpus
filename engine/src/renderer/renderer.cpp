#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>

#include "engine/systems.hpp"
#include "resources/resource_manager.hpp"
#include "ecs/components.hpp"
#include "engine/world.hpp"
#include "ecs/view.hpp"
#include "renderer/renderer.hpp"
#include "opengl/gl_backend.hpp"

void Renderer::init(World &world) {

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR]: glad load failed \n";
        return;
    }

    glViewport(0, 0, world.engineConfig.windowConfig.width, world.engineConfig.windowConfig.height);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    GLBackend::createUBO(world.engineState.renderState.lightUBO, sizeof(GPULightBlock), BlockBinding::LightBlock);
    GLBackend::createUBO(world.engineState.renderState.cameraUBO, sizeof(GPUCameraBlock), BlockBinding::CameraBlock);
}

GPULightBlock Renderer::gatherLightData(World& world) {

    GPULightBlock lightData;
    int count = 0;

    for (const auto [entity, light] : View<PointLight>(world.registry)) {

        if (count >= MaxPointLights) {
            break;
        }
        GPUPointLight pointLight;

        pointLight.color = glm::vec4(light.color, 1.0f);
        pointLight.position = glm::vec4(light.position, 1.0f);
        pointLight.intensity = light.intensity;
        pointLight.radius = light.radius;

        lightData.pointLights[count] = pointLight;
        count++;
    }
    lightData.pointLightCount = count;

    count = 0;
    for (const auto [entity, light] : View<Spotlight>(world.registry)) {

        if (count >= MaxSpotlights) {
            break;
        }
        GPUSpotlight spotlight;

        spotlight.color = glm::vec4(light.color, 1.0f);
        spotlight.position = glm::vec4(light.position, 1.0f);
        spotlight.direction = glm::vec4(light.direction, 0.0f);
        spotlight.intensity = light.intensity;
        spotlight.radius = light.radius;
        spotlight.innerCutOff = glm::cos(glm::radians(light.innerCutOff));
        spotlight.outerCutOff = glm::cos(glm::radians(light.outerCutOff));

        lightData.spotlights[count] = spotlight;
        count++;
    }
    lightData.spotlightCount = count;

    count = 0;
    GPUDirectionalLight dirLight;
    for (const auto [entity, light] : View<DirectionalLight>(world.registry)) {

        if (count >= MaxDirectionalLights) {
            break;
        }
        dirLight.color = glm::vec4(light.color, 1.0f);
        dirLight.direction = glm::vec4(light.direction, 0.0f);
        dirLight.intensity = light.intensity;
        dirLight.ambientStrength = light.ambientStrength;
        count++;
    }
    lightData.dirLight = dirLight;
    
    return lightData;
}

GPUCameraBlock Renderer::gatherCameraData(World &world) {

    Camera& camera = world.registry.getComponent<Camera>(world.activeCamera);

    GPUCameraBlock cameraData;
    cameraData.position = camera.position;

    CameraSystem::updateView(camera, cameraData.view);
    CameraSystem::updateProjection(world, camera, cameraData.projection);

    return cameraData;
}

void Renderer::renderMeshes(World& world) {

    for (auto [entity, mesh, material, renderable, worldTransform] : View<Mesh, Material, Renderable, WorldTransform>(world.registry)) {

        if (!renderable.visible) {
            continue;
        }

        const MaterialAsset& materialAsset = world.resourceManager.getMaterialAsset(material.handle);

        ShaderAsset& shaderAsset = world.resourceManager.getShaderAsset(materialAsset.shaderHandle);

        const Texture& diffuseMap = world.resourceManager.getTexture(materialAsset.diffuseMap);
        GLBackend::bindTextureUnit(diffuseMap.id, TextureUnit::DiffuseMap);

        const Texture& specularMap = world.resourceManager.getTexture(materialAsset.specularMap);
        GLBackend::bindTextureUnit(specularMap.id, TextureUnit::SpecularMap);

        const Texture& normalMap = world.resourceManager.getTexture(materialAsset.normalMap);
        GLBackend::bindTextureUnit(normalMap.id, TextureUnit::NormalMap);

        glm::mat4 modelMatrix = worldTransform.matrix;
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uModel"], modelMatrix);
        
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uColor"], materialAsset.baseColor);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uSpecularStrength"], materialAsset.specularStrength);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uShininess"], materialAsset.shininess);

        const GPUMesh& gpuMesh = world.resourceManager.getGPUMesh(mesh.handle);
        GLBackend::drawIndexed(gpuMesh, shaderAsset.shaderProgram);
    }
}

void Renderer::beginFrame(World &world) {
    
    GLBackend::clearBuffer(world.engineState.renderState.clearColor);

    GPULightBlock lightData = gatherLightData(world);
    GLBackend::updateUBO(world.engineState.renderState.lightUBO, sizeof(GPULightBlock), &lightData);

    GPUCameraBlock cameraData = gatherCameraData(world);
    GLBackend::updateUBO(world.engineState.renderState.cameraUBO, sizeof(GPUCameraBlock), &cameraData);
}

void Renderer::renderScene(World &world) {

    renderMeshes(world);
}