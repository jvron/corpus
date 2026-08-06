#include "assimp/matrix4x4.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <assimp/vector3.h>
#include <glm/geometric.hpp>
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "resources/asset_loader.hpp"

std::string AssetLoader::readFile(const std::string &filePath) {

    std::ifstream file;
    file.open(filePath);

    if (!file) {
        std::cerr << "[ERROR]: File not found at "<< filePath <<"\n";
        return "";
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}

ImageData AssetLoader::loadImage(const std::string& filePath) {

    ImageData imageData;
    int channels {};

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &imageData.width, &imageData.height, &channels, 0);

    if (data == nullptr) {
        std::cerr << "[ERROR](stb_image): Failed to load texture: " << filePath << "\n";
    }

    imageData.data = data;

    switch (channels) {
        case 1:
            imageData.format = TexFormat::Red;
            break;
        case 2:
            imageData.format = TexFormat::Rg;
            break;
        case 3:
            imageData.format = TexFormat::Rgb; 
            break;
        case 4:
            imageData.format = TexFormat::Rgba;
            break;
        default:
            std::cerr << "[ERROR](stb_image): Unsupported number of channels: " << channels <<", for texture: " << filePath << "\n";
            break;
    }  

    return imageData;
}

void AssetLoader::freeImage(ImageData& imageData) {

    if (imageData.data) {

        stbi_image_free(imageData.data);
        imageData.data = nullptr;
    }
}

static void buildVertexLayout(VertexLayout& layout, bool hasColor, bool hasUv, bool hasNormal, bool hasTangent) {

    VertexAttribute position = {
        .location = AttributeLocation::Position,
        .componentCount = 3,
        .type = ComponentType::Float,
        .normalized = false,
        .relativeOffset = offsetof(Vertex, position)
    };

    layout.attributes.push_back(position);

    if (hasColor) {
        VertexAttribute color = {
            .location = AttributeLocation::Color,
            .componentCount = 4,
            .type = ComponentType::Float,
            .normalized = false,
            .relativeOffset = offsetof(Vertex, color)
        };

        layout.attributes.push_back(color);
    }

    if (hasUv) {
        VertexAttribute uv = {
            .location = AttributeLocation::Uv,
            .componentCount = 2,
            .type = ComponentType::Float,
            .normalized = false,
            .relativeOffset = offsetof(Vertex, uv)
        };

        layout.attributes.push_back(uv);
    }

    if (hasNormal) {
        VertexAttribute normal = {
            .location = AttributeLocation::Normal,
            .componentCount = 3,
            .type = ComponentType::Float,
            .normalized = false,
            .relativeOffset = offsetof(Vertex, normal)
        };

        layout.attributes.push_back(normal);
    }

    if (hasTangent) {
        VertexAttribute tangent = {
            .location = AttributeLocation::Tangent,
            .componentCount = 4,
            .type = ComponentType::Float,
            .normalized = false,
            .relativeOffset = offsetof(Vertex, tangent)
        };

        layout.attributes.push_back(tangent);
    }
}

static MeshData processMesh(const aiMesh& mesh) {
    MeshData meshData;

    bool hasColor = mesh.HasVertexColors(0);
    bool hasUv = mesh.HasTextureCoords(0);
    bool hasNormal = mesh.HasNormals();
    bool hasTangent = mesh.HasTangentsAndBitangents() && hasNormal;

    meshData.vertices.reserve(mesh.mNumVertices);

    for (size_t i = 0; i < mesh.mNumVertices; i++) {
        Vertex vertex;

        const aiVector3D& pos = mesh.mVertices[i];
        vertex.position = {
            pos.x,
            pos.y,
            pos.z
        };

        if (hasColor) {
            const aiColor4D& color = mesh.mColors[0][i];
            vertex.color = {
                color.r,
                color.g,
                color.b,
                color.a
            };
        }
        else {
            vertex.color = {1.0f, 1.0f, 1.0f, 1.0f};
        }

        if (hasUv) {
            const aiVector3D& texCoords = mesh.mTextureCoords[0][i];
            vertex.uv = {
                texCoords.x,
                texCoords.y
            };
        }
        else {
            vertex.uv = {0.0f, 0.0f};
        }

        if (hasNormal) {
            const aiVector3D& norm = mesh.mNormals[i];
            vertex.normal = {
                norm.x,
                norm.y,
                norm.z
            };
        }
        else {
            vertex.normal = {0.0f, 0.0f, 0.0f};
        }

        if (hasTangent) {
            const aiVector3D& T = mesh.mTangents[i];
            const aiVector3D& B = mesh.mBitangents[i];
            const aiVector3D& N = mesh.mNormals[i];

            glm::vec3 tangent = {T.x, T.y, T.z};
            glm::vec3 biTangent = {B.x, B.y, B.z};
            glm::vec3 normal = {N.x, N.y, N.z};

            float w = glm::dot(glm::cross(normal, tangent), biTangent) < 0.0f ? -1.0f : 1.0f;

            vertex.tangent = {
                tangent.x,
                tangent.y,
                tangent.z,
                w
            };
        }
        else {
            vertex.tangent = {0.0f, 0.0f, 0.0f, 1.0f};
        }

        meshData.vertices.push_back(vertex);
    }

    meshData.indices.reserve(mesh.mNumFaces * 3);
    for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
        const aiFace& face = mesh.mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            meshData.indices.push_back(face.mIndices[j]);
        }
    }

    VertexLayout layout;
    buildVertexLayout(layout, hasColor, hasUv, hasNormal, hasTangent);

    layout.bindingIndex = 0;
    layout.stride = sizeof(Vertex);
    meshData.vertexLayout = layout;

    return meshData;
}

static void processTexture(const aiMaterial& material, MaterialImport& materialImport, const std::filesystem::path& modelDir, aiTextureType aiTexType, TexType texType) {

    size_t textureCount = material.GetTextureCount(aiTexType);

    materialImport.textureImports.reserve(materialImport.textureImports.size() + textureCount);

    for (size_t i = 0; i < textureCount; i++) {
        
        aiString path;
        material.GetTexture(aiTexType, i, &path);

        TextureImport textureImport;
        textureImport.type = texType;
        textureImport.path = modelDir / path.C_Str();

        materialImport.textureImports.push_back(std::move(textureImport));
    }
}

static void processMaterials(const aiScene& scene, SceneImport& sceneImport, const std::filesystem::path& modelDir) {

    sceneImport.materialImports.reserve(scene.mNumMaterials);

    for (size_t i = 0; i < scene.mNumMaterials; i++) {
        const aiMaterial* material = scene.mMaterials[i];

        MaterialImport materialImport;

        std::string name = material->GetName().C_Str();
        if (name.empty()) {
            materialImport.name = "Material_" + std::to_string(i);
        }
        else {
            materialImport.name = std::move(name);
        }

        processTexture(*material, materialImport,  modelDir, aiTextureType_DIFFUSE, TexType::DiffuseMap);
        processTexture(*material, materialImport, modelDir, aiTextureType_SPECULAR, TexType::SpecularMap);
        processTexture(*material, materialImport, modelDir, aiTextureType_NORMALS, TexType::NormalMap);
        processTexture(*material, materialImport, modelDir, aiTextureType_HEIGHT, TexType::NormalMap);

        sceneImport.materialImports.push_back(std::move(materialImport));
    }
}

static NodeImport processNode(const aiNode& node, const aiScene& scene) {

    NodeImport nodeImport;
    nodeImport.name = node.mName.C_Str(); 

    aiVector3D scale;
    aiVector3D rotation;
    aiVector3D position;

    node.mTransformation.Decompose(scale, rotation, position);

    nodeImport.localTransform.scale = {
        scale.x,
        scale.y,
        scale.z 
    };

    nodeImport.localTransform.rotation = {
        rotation.x,
        rotation.y,
        rotation.z,
    };

    nodeImport.localTransform.position = {
        position.x,
        position.y,
        position.z
    };

    for (size_t i = 0; i < node.mNumMeshes; i++) {

        const aiMesh* mesh =  scene.mMeshes[node.mMeshes[i]];

        std::string name = mesh->mName.C_Str();

        if (name.empty()) {
            name = "Mesh_" + std::to_string(nodeImport.meshImports.size());
        }
        

        MeshImport meshImport {
            .name = std::move(name),
            .meshData = processMesh(*mesh),
            .materialIndex = mesh->mMaterialIndex
        };
        nodeImport.meshImports.push_back(std::move(meshImport));
    }

    //process child nodes
    for (size_t i = 0; i < node.mNumChildren; i++) {

        NodeImport childNode = processNode(*node.mChildren[i], scene);

        nodeImport.children.push_back(std::make_unique<NodeImport>(std::move(childNode)));
    }

    return nodeImport;
}

SceneImport AssetLoader::loadScene(const std::string& filePath) {

    constexpr unsigned int importFlags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs;

    SceneImport sceneImport;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filePath, importFlags);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "[ERROR](Assimp): " << importer.GetErrorString() << "\n";
        return sceneImport;
    }

    std::filesystem::path path = filePath;
    sceneImport.name = path.stem().string();

    std::filesystem::path modelDir = path.parent_path();

    processMaterials(*scene, sceneImport, modelDir);

    sceneImport.root = processNode(*scene->mRootNode, *scene);

    return sceneImport;
}