#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "resources/resource_manager.hpp"
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

void ResourceManager::createShaderProgram(const std::string &programName, std::vector<std::string> &filePaths) {

    uint32_t shaderProgram = GLBackend::createShaderProgram();
    
    for (const auto &path : filePaths) {

        std::string shaderSource = readFile(path);

        if (shaderSource.empty()) {
            std::cerr << "Error: Skipping shader program " << programName << " due to missing file: " << path << "\n";
            continue;;
        }
        const char* c_shaderSouce = shaderSource.c_str();

        std::filesystem::path filePath = path;

        uint32_t shader {};

        if (filePath.extension() == ".vert") {
            shader = GLBackend::compileVertShader(c_shaderSouce);
        }
        else if (filePath.extension() == ".frag") {
            shader = GLBackend::compileFragShader(c_shaderSouce);
        }
        else {
            std::cerr << "Error: Unkown shader type " << path << "\n";
            continue;
        }

        GLBackend::attachShader(shaderProgram, shader);
        GLBackend::deleteShader(shader);
    }

    GLBackend::linkProgram(shaderProgram);

    shaderPrograms.insert({programName, shaderProgram});
}

uint32_t ResourceManager::getShaderProgram(const std::string &programName) {

    if (shaderPrograms.find(programName) == shaderPrograms.end()) {
        std::cerr << "Error: Shader program called " << programName << " does not exist"<<"\n";
    }

    return shaderPrograms.at(programName);
}

Mesh ResourceManager::insertMeshAsset(const MeshAsset &meshAsset) {

    uint32_t index = meshAssets.size();
    meshAssets.push_back(meshAsset);

    Mesh mesh = {index};
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