#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "assets/asset_manager.hpp"
#include "renderer/opengl/gl_backend.hpp"

std::string AssetManager::readFile(const std::string &filePath) {

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


void AssetManager::createShaderProgram(const std::string &programName, std::vector<std::string> filePaths) {

    uint32_t shaderProgram = GLBackend::createShaderProgram();

    for (int i = 0; i < filePaths.size(); i++) {

        std::string shaderSource = readFile(filePaths[i]);

        if (shaderSource.empty()) {
            std::cerr << "Error: Skipping shader program " << programName << " due to missing file: " << filePaths[i] << "\n";
            break;
        }
        const char* c_shaderSouce = shaderSource.c_str();

        std::filesystem::path filePath = filePaths[i];

        uint32_t shader {};

        if (filePath.extension() == ".vert") {
            shader = GLBackend::compileVertShader(c_shaderSouce);
        }
        else if (filePath.extension() == ".frag") {
            shader = GLBackend::compileFragShader(c_shaderSouce);
        }

        GLBackend::attachShader(shaderProgram, shader);
        GLBackend::deleteShader(shader);
    }

    GLBackend::linkProgram(shaderProgram);

    shaderPrograms.insert({programName, shaderProgram});
}

uint32_t AssetManager::getShaderProgram(const std::string &programName) {

    if (shaderPrograms.find(programName) == shaderPrograms.end()) {
        std::cerr << "Error: Shader program called " << programName << " does not exist"<<"\n";
    }

    return shaderPrograms.at(programName);
}