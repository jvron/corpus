#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "resources/resource_manager.hpp"
#include "ecs/components.hpp"
#include "renderer/opengl/gl_backend.hpp"

std::string ResourceManager::readFile(const std::string &filePath) {

    std::ifstream file;
    file.open(filePath);
    
    if (!file) {
        std::cerr << "Error: File not found at "<< filePath <<"\n";
        return "";
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}

ShaderHandle ResourceManager::createShaderProgram(std::vector<std::string> &filePaths) {

    ShaderProgram program = GLBackend::createShaderProgram();
    
    for (const auto &path : filePaths) {

        std::string shaderSource = readFile(path);

        if (shaderSource.empty()) {
            std::cerr << "Error: Skipping shader program " << " due to missing file: " << path << "\n";
            continue;;
        }
        const char* c_shaderSource = shaderSource.c_str();

        std::filesystem::path filePath = path;

        uint32_t shader {};

        if (filePath.extension() == ".vert") {
            shader = GLBackend::compileVertShader(c_shaderSource);
        }
        else if (filePath.extension() == ".frag") {
            shader = GLBackend::compileFragShader(c_shaderSource);
        }
        else {
            std::cerr << "Error: Unkown shader type " << path << "\n";
            continue;
        }

        GLBackend::attachShader(program, shader);
        GLBackend::deleteShader(shader);
    }

    GLBackend::linkProgram(program);

    ShaderHandle handle = shaderPrograms.size();
    shaderPrograms.push_back(program);
    return handle;
}

ShaderProgram ResourceManager::getShaderProgram(ShaderHandle shaderHandle) {

    assert(!(shaderHandle >= meshAssets.size())&& "Error: Shader program does not exist");

    return shaderPrograms[shaderHandle];
}

Mesh ResourceManager::insertMeshAsset(const MeshAsset &meshAsset) {

    MeshHandle handle = meshAssets.size();
    meshAssets.push_back(meshAsset);

    Mesh mesh = {.meshHandle = handle};
    return mesh;
}

MeshAsset& ResourceManager::getMeshAsset(MeshHandle meshHandle) {

    assert(!(meshHandle >= meshAssets.size()) && "Error: MeshAsset does not exist");
    return meshAssets[meshHandle];
}   

void ResourceManager::insertGPUMesh(MeshHandle meshHandle, const GPUMesh &gpuMesh) {

    if (meshHandle >= gpuMeshes.size()) {
        gpuMeshes.resize(gpuMeshes.size() + 1);
    }
    gpuMeshes[meshHandle] = gpuMesh;
}

GPUMesh& ResourceManager::getGPUMesh(MeshHandle meshHandle) {
    assert(!(meshHandle >= gpuMeshes.size()) && "Error: GPUMesh does not exist");

    return gpuMeshes[meshHandle]; 
}