#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <vector>

#include "resources/resource_manager.hpp"
#include "ecs/components.hpp"
#include "engine/world.hpp"
#include "ecs/view.hpp"
#include "renderer/renderer.hpp"
#include "opengl/gl_backend.hpp"

void Renderer::init(World &world) {

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error: glad load failed \n";
        return;
    }

    glViewport(0, 0, world.engineConfig.windowConfig.width, world.engineConfig.windowConfig.height);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void Renderer::uploadMesh(World &world) {

    View view = View<Mesh>(world.registry);
    const std::vector<Entity> &validEntities = view.getEntities();

    for (const Entity &entity : validEntities) {
        Mesh &mesh = world.registry.getComponent<Mesh>(entity);
       
        std::cout << "Debug: UploadMesh found " << validEntities.size() << " entities\n";
        MeshAsset &meshAsset = world.resourceManager.getMeshAsset(mesh.meshHandle);

        GPUMesh gpuMesh;
        GLBackend::createVertexArray(gpuMesh.vao);
        GLBackend::createBuffer(gpuMesh.vbo);
        GLBackend::createBuffer(gpuMesh.ebo);
        gpuMesh.indexCount = meshAsset.indices.size();

        std::cout << "Debug: Uploading mesh indices size: " << meshAsset.indices.size() << "\n";
        std::cout << "Debug: Uploading mesh vertices size: " << meshAsset.vertices.size() << "\n";

        GLBackend::uploadBuffer(gpuMesh.vbo, meshAsset.vertices.size() * sizeof(Vertex), meshAsset.vertices.data());
        GLBackend::uploadBuffer(gpuMesh.ebo, meshAsset.indices.size() * sizeof(uint32_t), meshAsset.indices.data());

        GLBackend::attachVertexBuffer(gpuMesh.vao, meshAsset.vertexLayout.bindingIndex, gpuMesh.vbo, 0, meshAsset.vertexLayout.stride);
        GLBackend::attachElementBuffer(gpuMesh.vao, gpuMesh.ebo);

        for (const VertexAttribute &attribute : meshAsset.vertexLayout.attributes) {
            GLBackend::setAttribute(gpuMesh.vao, meshAsset.vertexLayout.bindingIndex, attribute);
        }

        world.resourceManager.insertGPUMesh(mesh.meshHandle, gpuMesh);
    }
}

void Renderer::beginFrame(World &world) {
    
    GLBackend::clearBuffer(world.engineState.renderState.clearColor);
}

void Renderer::renderScene(World &world) {

    View view = View<Mesh, Material, Renderable>(world.registry);
    const std::vector<Entity> &validEntities = view.getEntities();

    for (const Entity &entity : validEntities) {
        Material &material = world.registry.getComponent<Material>(entity);
        
        ShaderProgram shaderProgram = world.resourceManager.getShaderProgram(material.shaderHandle);

        Renderable &renderableObj = world.registry.getComponent<Renderable>(entity);

        if (!renderableObj.visible) {
            continue;
        }
        Mesh &mesh = world.registry.getComponent<Mesh>(entity);

        GPUMesh &gpuMesh = world.resourceManager.getGPUMesh(mesh.meshHandle);
        GLBackend::drawIndexed(gpuMesh, shaderProgram);
    }
}