#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include "ecs/components.hpp"
#include "asset_loader.hpp"
#include "renderer/opengl/gl_backend.hpp"

struct World;

using ShaderProgram = uint32_t;

enum class ShaderType {
    Vertex,
    Fragment,
};

struct MeshAsset {
    std::vector<Vertex> vertices {};
    std::vector<uint32_t> indices {};
    VertexLayout vertexLayout;
};

struct GPUMesh {
    uint32_t vao {};
    uint32_t vbo {};
    uint32_t ebo {};

    uint32_t indexCount {};
};

struct ShaderAsset {
    ShaderProgram shaderProgram {};
    std::unordered_map<std::string, int> uniformLocations; 
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