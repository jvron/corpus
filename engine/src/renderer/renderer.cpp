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

    GLBackend::createUBO(world.engineState.renderState.lightUBO, sizeof(GPULightBlock), BlockBinding::lightBlock);
    GLBackend::createUBO(world.engineState.renderState.cameraUBO, sizeof(GPUCameraBlock), BlockBinding::cameraBlock);
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

        if (count >= 1) {
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

void Renderer::beginFrame(World &world) {
    
    GLBackend::clearBuffer(world.engineState.renderState.clearColor);

    GPULightBlock lightData = gatherLightData(world);
    GLBackend::updateUBO(world.engineState.renderState.lightUBO, sizeof(GPULightBlock), &lightData);

    GPUCameraBlock cameraData = gatherCameraData(world);
    GLBackend::updateUBO(world.engineState.renderState.cameraUBO, sizeof(GPUCameraBlock), &cameraData);
}

void Renderer::renderScene(World &world) {

    for (const auto [entity, mesh, material, renderable, transform] : View<Mesh, Material, Renderable, Transform>(world.registry)) {

        if (!renderable.visible) {
            continue;
        }

        ShaderAsset& shaderAsset = world.resourceManager.getShaderAsset(material.shaderHandle);

        const Texture& diffuseMap = world.resourceManager.getTexture(material.diffuseMap);
        GLBackend::bindTextureUnit(diffuseMap.id, TextureUnit::diffuseMap);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["diffuseMap"], TextureUnit::diffuseMap);

        const Texture& specularMap = world.resourceManager.getTexture(material.specularMap);
        GLBackend::bindTextureUnit(specularMap.id, TextureUnit::specularMap);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["specularMap"], TextureUnit::specularMap);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, transform.scale);
        //Model = T * R * S
        
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uModel"], model);
        
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uColor"], material.baseColor);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uSpecularStrength"], material.specularStrength);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uShininess"], material.shininess);
    
        GPUMesh& gpuMesh = world.resourceManager.getGPUMesh(mesh.meshHandle);
        GLBackend::drawIndexed(gpuMesh, shaderAsset.shaderProgram);
    }
}