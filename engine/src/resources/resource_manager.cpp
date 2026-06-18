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

ShaderHandle ResourceManager::createShaderProgram(const std::vector<std::string> &filePaths) {

    ShaderProgram program = GLBackend::createShaderProgram();
    
    for (const auto &path : filePaths) {

        std::string shaderSource = readFile(path);

        if (shaderSource.empty()) {
            std::cerr << "[ERROR]: Skipping shader program " << " due to missing file: " << path << "\n";
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
            std::cerr << "[ERROR]: Unkown shader type " << path << "\n";
            continue;
        }

        GLBackend::attachShader(program, shader);
        GLBackend::deleteShader(shader);
    }

    GLBackend::linkProgram(program);

    ShaderHandle handle = shaderPrograms.size();
    shaderPrograms.push_back(program);

    ShaderAsset shaderAsset;
    shaderAsset.shaderProgram = program;
    shaderAssets.push_back(shaderAsset);

    return handle;
}

ShaderProgram ResourceManager::getShaderProgram(ShaderHandle shaderHandle) {

    assert(!(shaderHandle >= shaderPrograms.size()) && "[ERROR]: Shader program does not exist");

    return shaderPrograms[shaderHandle];
}



ShaderAsset& ResourceManager::getShaderAsset(ShaderHandle shaderHandle) {
    assert(!(shaderHandle >= shaderPrograms.size()) && "[ERROR]: Shader program does not exist");
    return shaderAssets[shaderHandle];
}



void ResourceManager::setUniformLocation(ShaderHandle shaderHandle, const std::string& uniformName) {

    ShaderAsset& shaderAsset =  getShaderAsset(shaderHandle);

    if (shaderAsset.uniformLocations.find(uniformName) == shaderAsset.uniformLocations.end()) {
        GLuint location = GLBackend::getUniformLocation(shaderAsset.shaderProgram, uniformName);
        //std::cerr << "[DEBUG]: Uniform name: " << uniformName << ", location: " << location << "\n";
        shaderAsset.uniformLocations[uniformName] = location;
    }
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