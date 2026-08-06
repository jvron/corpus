#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "ecs/components.hpp"

enum class TexFormat {
    Red,
    Rg,
    Rgb,
    Rgba
};

enum class TexType {
    DiffuseMap,
    SpecularMap,
    NormalMap,
    Albedo,
    Metallic,
    Unknown
};

enum class ComponentType {
    Int,
    Float,
    UnsignedInt,
    UnsignedByte
};

namespace AttributeLocation {
    constexpr uint32_t Position = 0;
    constexpr uint32_t Color = 1;
    constexpr uint32_t Uv = 2;
    constexpr uint32_t Normal = 3;
    constexpr uint32_t Tangent = 4;
}

struct Vertex {
    glm::vec3 position {};
    glm::vec4 color {};
    glm::vec2 uv {};
    glm::vec3 normal {};
    glm::vec4 tangent {};
};

struct VertexAttribute {
    uint32_t location {};
    uint32_t componentCount {};
    ComponentType type {};
    bool normalized {};
    size_t relativeOffset {};
};

struct VertexLayout {
    uint32_t bindingIndex {};
    size_t stride {};
    std::vector<VertexAttribute> attributes;
};

struct MeshData {
    std::vector<Vertex> vertices {};
    std::vector<uint32_t> indices {};
    VertexLayout vertexLayout;
};

struct MeshImport {
    std::string name;
    MeshData meshData;
    uint32_t materialIndex {};
};

struct ImageData {
    int width {};
    int height {};
    void* data = nullptr;
    TexFormat format {};
};

struct TextureImport {
    TexType type;
    std::string path;
};

struct MaterialImport {
    std::string name;
    std::vector<TextureImport> textureImports;
};

struct NodeImport {
    std::string name;
    Transform localTransform;
    std::vector<MeshImport> meshImports;
    std::vector<std::unique_ptr<NodeImport>> children;
};

struct ModelImport {
    std::string name;
    NodeImport root;
    std::vector<MaterialImport> materialImports;
};

namespace AssetLoader  {

    std::string readFile(const std::string& filePath);
    ImageData loadImage(const std::string& filePath);
    void freeImage(ImageData& imageData);

    ModelImport loadModel(const std::string& filePath);
}