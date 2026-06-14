#pragma once
#include <GL/gl.h>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include "ecs/components.hpp"

using MeshHandle = uint32_t;
typedef unsigned int GLuint ;

enum class ShaderType {
    Vertex,
    Fragment,
};

struct Vertex {
    Position position;
};

struct VertexAttribute {
    uint32_t location {};
    uint32_t componentCount {};
    GLenum componentType {};
    GLboolean normalized {};
    size_t relativeOffset {};
};

struct VertexLayout {
    uint32_t bindingIndex {};
    size_t stride {};
    std::vector<VertexAttribute> attributes;
};

struct MeshAsset {
    std::vector<Vertex> vertices {};
    std::vector<uint32_t> indices {};

    VertexLayout vertexLayout;
};

struct GPUMesh {
    GLuint vao {};
    GLuint vbo {};
    GLuint ebo {};

    uint32_t indexCount {};
};

class ResourceManager {

private:
    std::unordered_map<std::string, uint32_t> shaderPrograms;
    std::vector<MeshAsset> meshAssets;
    
    std::string readFile(const std::string &filePath);
    
public:
    std::vector<GPUMesh> gpuMeshes;

    void createShaderProgram(const std::string &programName, std::vector<std::string> &shaderPaths);
    uint32_t getShaderProgram(const std::string &programName);

    Mesh insertMeshAsset(const MeshAsset &meshAsset);
    MeshAsset& getMeshAsset(MeshHandle meshHandle);

    void insertGPUMesh(MeshHandle meshHandle, const GPUMesh &gpuMesh);
    GPUMesh& getGPUMesh(MeshHandle meshHandle);
};