#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <glm/ext/matrix_transform.hpp>

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

void Renderer::beginFrame(World &world) {
    
    GLBackend::clearBuffer(world.engineState.renderState.clearColor);

    GPULightBlock lightData = gatherLightData(world);
    GLBackend::updateUBO(world.engineState.renderState.lightUBO, sizeof(GPULightBlock), &lightData);
}

void Renderer::renderScene(World &world) {

    Camera& camera = world.registry.getComponent<Camera>(world.activeCamera);

    glm::mat4 view = glm::mat4(1.0f);
    CameraSystem::updateView(camera, view);
    
    glm::mat4 projection = glm::mat4(1.0f);
    CameraSystem::updateProjection(world, camera, projection);

    for (const auto& [entity, mesh, material, renderable, transform] : View<Mesh, Material, Renderable, Transform>(world.registry)) {

        if (!renderable.visible) {
            continue;
        }

        glm::mat4 model = glm::mat4(1.0f);

        ShaderAsset& shaderAsset = world.resourceManager.getShaderAsset(material.shaderHandle);

        Texture& texture = world.resourceManager.getTexture(material.textureHandle);
        int textureUnit = 0;
        GLBackend::bindTextureUnit(texture.id, textureUnit);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uTexture"], textureUnit);

        model = glm::translate(model, transform.position);
        model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, transform.scale);
        //Model = T * R * S
        
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uModel"], model);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uView"], view);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uProjection"], projection);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uColor"], material.baseColor);

        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uSpecularStrength"], material.specularStrength);
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uShine"], material.shine);
        
        GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["uViewPos"], camera.position);
    
        GPUMesh& gpuMesh = world.resourceManager.getGPUMesh(mesh.meshHandle);
        GLBackend::drawIndexed(gpuMesh, shaderAsset.shaderProgram);
    }
}