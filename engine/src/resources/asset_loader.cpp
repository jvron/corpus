#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assimp/vector3.h>
#include <glm/geometric.hpp>
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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

void buildVertexLayout(VertexLayout& layout, bool hasColor, bool hasUv, bool hasNormal, bool hasTangent) {

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

MeshData processMesh(const aiMesh& mesh) {
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
            vertex.tangent = {0.0f, 0.0f, 0.0f, 0.0f};
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

void processTexture(const aiMaterial& material, MaterialImport& materialImport, const std::filesystem::path& modelDir, aiTextureType aiTexType, TexType texType) {

    size_t textureCount = material.GetTextureCount(aiTexType);

    materialImport.textureImports.reserve(materialImport.textureImports.size() + textureCount);

    for (size_t i = 0; i < textureCount; i++) {
        
        aiString path;
        material.GetTexture(aiTexType, i, &path);

        std::filesystem::path texturePath = modelDir / path.C_Str();

        ImageData imageData = AssetLoader::loadImage(texturePath.string());

        TextureImport textureImport;
        textureImport.imageData = imageData;
        textureImport.type = texType;
        textureImport.path = std::move(texturePath);

        materialImport.textureImports.push_back(std::move(textureImport));
    }
}

void processMaterials(const aiScene& scene, ModelImport& modelImport, const std::filesystem::path& modelDir) {

    modelImport.materialImports.reserve(scene.mNumMaterials);

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

        modelImport.materialImports.push_back(std::move(materialImport));
    }
}

void processNode(const aiNode& node, const aiScene& scene, ModelImport& modelImport) {

    for (size_t i = 0; i < node.mNumMeshes; i++) {
        const aiMesh* mesh =  scene.mMeshes[node.mMeshes[i]];

        std::string name = mesh->mName.C_Str();

        if (name.empty()) {
            name = "Mesh_" + std::to_string(modelImport.meshImports.size());
        }

        MeshImport meshImport = {
            .name = std::move(name),
            .meshData = processMesh(*mesh),
            .materialIndex = mesh->mMaterialIndex
        };
        modelImport.meshImports.push_back(std::move(meshImport));
    }

    //process child nodes
    for (size_t i = 0; i < node.mNumChildren; i++) {
        processNode(*node.mChildren[i], scene, modelImport);
    }
}

ModelImport AssetLoader::loadModel(const std::string& filePath) {

    constexpr unsigned int importFlags = aiProcess_Triangulate | aiProcess_FlipUVs | 
                                        aiProcess_GenNormals | aiProcess_CalcTangentSpace;
    ModelImport modelImport;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filePath, importFlags);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "[ERROR](Assimp): " << importer.GetErrorString() << "\n";
        return modelImport;
    }

    std::filesystem::path path = filePath;
    modelImport.name = path.stem().string();

    std::filesystem::path modelDir = path.parent_path();

    processMaterials(*scene, modelImport, modelDir);
    processNode(*scene->mRootNode, *scene, modelImport);

    return modelImport;
}