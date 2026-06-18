#pragma once

#include <GL/gl.h>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include "ecs/components.hpp"

using ShaderProgram = uint32_t;

enum class ShaderType {
    Vertex,
    Fragment,
};

struct Vertex {
    glm::vec3 position {};
    glm::vec4 color {};
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

struct ShaderAsset {
    ShaderProgram shaderProgram {};
    std::unordered_map<std::string, GLint> uniformLocations; 
};

class ResourceManager {

private:
    std::vector<ShaderProgram> shaderPrograms;
    std::vector<ShaderAsset> shaderAssets;
    std::vector<MeshAsset> meshAssets;

    std::vector<GPUMesh> gpuMeshes;

    std::string readFile(const std::string& filePath);
    
public:

    ShaderHandle createShaderProgram(const std::vector<std::string> &shaderPaths);
    ShaderProgram getShaderProgram(ShaderHandle shaderHandle);

    ShaderAsset& getShaderAsset(ShaderHandle shaderHandle);
    void setUniformLocation(ShaderHandle shaderHandle, const std::string& uniformName);

    Mesh insertMeshAsset(const MeshAsset &meshAsset);
    MeshAsset& getMeshAsset(MeshHandle meshHandle);

    void insertGPUMesh(MeshHandle meshHandle, const GPUMesh &gpuMesh);
    GPUMesh& getGPUMesh(MeshHandle meshHandle);
};