#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>

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

    for (const auto& [entity, light, transform] : View<PointLight, Transform>(world.registry)) {

        GPUPointLight pointLight;
        if (count >= 32) {
            break;
        }

        pointLight.color = glm::vec4(light.color, light.ambientStrength);
        pointLight.position = glm::vec4(transform.position, light.radius);

        lightData.pointLight[count] = pointLight;
        count++;
    }
    lightData.lightCount = count;
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