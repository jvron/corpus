#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include "ecs/components.hpp"
#include "asset_loader.hpp"

struct World;

using ShaderProgram = uint32_t;

enum class ShaderType {
    Vertex,
    Fragment,
};

enum class TexWrap {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

enum class TexFilter {
    Linear,
    Nearest,

    // mipmap filters 
    NearestMipmapNearest,
    NearestMipmapLinear,

    LinearMipmapLinear,
    LinearMipmapNearest
};

struct GPUMesh {
    uint32_t vao {};
    uint32_t vbo {};
    uint32_t ebo {};
    
    uint32_t indexCount {};
};

struct MeshAsset {
    std::string name;
    std::optional<MeshData> meshData;
    GPUMesh gpuMesh;
};

struct ShaderAsset {
    ShaderProgram shaderProgram {};
    std::unordered_map<std::string, int> uniformLocations; 
};

struct Texture {
    uint32_t id {};
};

struct Tex2DParameters {
    TexWrap wrapS;
    TexWrap wrapT;
    TexFilter minFilter;
    TexFilter magFilter;

    bool enableMipmap {true};
};

struct MaterialAsset {
    std::string name;

    ShaderHandle shaderHandle {};

    TextureHandle diffuseMap {};
    TextureHandle specularMap {};

    glm::vec4 baseColor {1.0f};
    float specularStrength {};
    float shininess {};
};

struct ModelOptions {

    bool storeMeshData {false};

    ShaderHandle shader {};

    Tex2DParameters diffuseParameters;
    Tex2DParameters specularParameters;

    float materialShininess {};
    float materialSpecularStrength {};
};

class ResourceManager {

private:
    std::vector<ShaderProgram> shaderPrograms;
    std::vector<ShaderAsset> shaderAssets;
    std::vector<Texture> textures;

    std::vector<MeshAsset> meshAssets;
    std::vector<MaterialAsset> materialAssets;
    
public:

    ShaderHandle createShaderProgram(const std::vector<std::string>& shaderPaths);
    ShaderProgram getShaderProgram(ShaderHandle shaderHandle);

    ShaderAsset& getShaderAsset(ShaderHandle shaderHandle);
    void setUniformLocation(ShaderHandle shaderHandle, const std::string& uniformName);

    Mesh loadMesh(const MeshData& meshData, const std::string meshName, bool storeMeshData = false);
    void insertMeshAsset(MeshHandle meshHandle, const MeshAsset& meshAsset);
    MeshAsset& getMeshAsset(MeshHandle meshHandle);

    GPUMesh buildGPUMesh(const MeshData& meshData);
    GPUMesh& getGPUMesh(MeshHandle meshHandle);

    TextureHandle createTexture(const ImageData& imageData);
    TextureHandle loadTexture(const std::string& texturePath);
    Texture& getTexture(TextureHandle textureHandle);
    void setTex2DParameters(TextureHandle textureHandle, const Tex2DParameters& parameters);

    Model loadModel(const std::string& filePath, const ModelOptions& modelOptions);
    MaterialAsset& getMaterialAsset(MaterialHandle materialHandle);

    void destroy();
};