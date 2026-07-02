#pragma once

#include <GL/gl.h>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include "ecs/components.hpp"
#include "renderer/opengl/gl_backend.hpp"

struct World;

using ShaderProgram = uint32_t;

enum class ShaderType {
    Vertex,
    Fragment,
};

struct Vertex {
    glm::vec3 position {};
    glm::vec4 color {};
    glm::vec2 uv {};
    glm::vec3 normal {};
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

struct Texture {
    uint32_t id {};
};

struct Texture2DParam {
    TexWrap wrapS;
    TexWrap wrapT;
    TexFilter minFilter;
    TexFilter magFilter;

    bool enableMipmap {true};
};

class ResourceManager {

private:
    std::vector<ShaderProgram> shaderPrograms;
    std::vector<ShaderAsset> shaderAssets;
    std::vector<MeshAsset> meshAssets;
    std::vector<Texture> textures;

    std::vector<GPUMesh> gpuMeshes;
    
public:

    ShaderHandle createShaderProgram(const std::vector<std::string>& shaderPaths);
    ShaderProgram getShaderProgram(ShaderHandle shaderHandle);

    ShaderAsset& getShaderAsset(ShaderHandle shaderHandle);
    void setUniformLocation(ShaderHandle shaderHandle, const std::string& uniformName);

    Mesh insertMeshAsset(const MeshAsset& meshAsset);
    MeshAsset& getMeshAsset(MeshHandle meshHandle);

    void insertGPUMesh(MeshHandle meshHandle, const GPUMesh& gpuMesh);
    void buildGPUMesh(World& world);
    GPUMesh& getGPUMesh(MeshHandle meshHandle);

    TextureHandle loadTexture(const std::string& texturePath);
    Texture& getTexture(TextureHandle textureHandle);
    void setTexture2DParameters(TextureHandle textureHandle, const Texture2DParam& parameters);

    void destroy();
};