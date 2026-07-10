#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
        std::cerr << "[ERROR]: Failed to load texture: " << filePath << "\n";
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
            std::cerr << "[ERROR]: Unsupported number of channels: " << channels << "\n";
            break;
    }

    return imageData;
} 

void AssetLoader::freeImage(void* data) {
    stbi_image_free(data);
}

void buildVertexLayout(VertexLayout& layout, bool hasColor, bool hasUv, bool hasNormal) {

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
}

MeshData processMesh(const aiMesh& mesh) {
    MeshData meshData;

    bool hasColor = false;
    bool hasUv = false;
    bool hasNormal = false;

    meshData.vertices.reserve(mesh.mNumVertices);

    for (size_t i = 0; i < mesh.mNumVertices; i++) {
        Vertex vertex;

        const aiVector3D& pos = mesh.mVertices[i];
        vertex.position = {
            pos.x,
            pos.y,
            pos.z
        };

        if (mesh.HasNormals()) {
            const aiVector3D& norm = mesh.mNormals[i];
            vertex.normal = {
                norm.x,
                norm.y,
                norm.z
            };
            hasNormal = true;
        }
        else {
            vertex.normal = {0.0f, 0.0f, 0.0f};
        }

        if (mesh.mColors[0]) {
            const aiColor4D& color = mesh.mColors[0][i];
            vertex.color = {
                color.r,
                color.g,
                color.b,
                color.a
            };

            hasColor = true;
        }
        else {
            vertex.color = {1.0f, 1.0f, 1.0f, 1.0f};
        }
        
        if (mesh.mTextureCoords[0]) {

            const aiVector3D& texCoords = mesh.mTextureCoords[0][i];
            vertex.uv = {
                texCoords.x, 
                texCoords.y
            };

            hasUv = true;
        }
        else {
            vertex.uv = {0.0f, 0.0f};
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
    buildVertexLayout(layout, hasColor, hasUv, hasNormal);  
    layout.bindingIndex = 0;
    layout.stride = sizeof(Vertex);      
    meshData.vertexLayout = layout;

    return meshData;
}

void processTexture(const aiMaterial& material, MaterialImport& materialImport, aiTextureType aiTexType, TexType texType) {

    size_t textureCount = material.GetTextureCount(aiTexType);

    materialImport.textureImports.reserve(materialImport.textureImports.size() + textureCount);

    for (size_t i = 0; i < textureCount; i++) {
        aiString path;
        material.GetTexture(aiTexType, i, &path);

        ImageData imageData = AssetLoader::loadImage(path.C_Str());

        TextureImport textureImport;
        textureImport.imageData = imageData;
        textureImport.type = texType;
        textureImport.path = path.C_Str();

        materialImport.textureImports.push_back(textureImport);
    }
}

void processMaterials(const aiScene& scene, ModelImport& modelImport) {

    modelImport.materialImports.reserve(scene.mNumMaterials);

    for (size_t i = 0; i < scene.mNumMaterials; i++) {    
        const aiMaterial* material = scene.mMaterials[i];

        MaterialImport materialImport;
        processTexture(*material, materialImport,  aiTextureType_DIFFUSE, TexType::DiffuseMap);
        processTexture(*material, materialImport, aiTextureType_SPECULAR, TexType::SpecularMap);

        modelImport.materialImports.push_back(materialImport);
    }
}

void processNode(const aiNode& node, const aiScene& scene, ModelImport& modelImport) {

    for (size_t i = 0; i < node.mNumMeshes; i++) {
        const aiMesh* mesh =  scene.mMeshes[node.mMeshes[i]];

        MeshImport meshImport = {
            .meshData = processMesh(*mesh), 
            .materialIndex = mesh->mMaterialIndex
        };
        modelImport.meshImports.push_back(meshImport);
    }

    //process child nodes
    for (size_t i = 0; i < node.mNumChildren; i++) {
        processNode(*node.mChildren[i], scene, modelImport);
    }
}

ModelImport AssetLoader::loadModel(const std::string& filePath) {

    constexpr unsigned int importFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals;

    ModelImport modelImport;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filePath, importFlags);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "[ERROR](Assimp): " << importer.GetErrorString() << "\n";
        return modelImport;
    }

    processMaterials(*scene, modelImport);
    processNode(*scene->mRootNode, *scene, modelImport);

    return modelImport;
}