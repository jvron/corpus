#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

enum class ShaderType {
    Vertex,
    Fragment,
};

class AssetManager {

private:
    std::unordered_map<std::string, uint32_t> shaderPrograms;
    std::string readFile(const std::string &filePath);

public:

    void createShaderProgram(const std::string &programName, std::vector<std::string> shaderPaths);
    uint32_t getShaderProgram(const std::string &programName);
};