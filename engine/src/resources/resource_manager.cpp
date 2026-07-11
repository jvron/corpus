#include <cassert>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "resources/resource_manager.hpp"
#include "ecs/components.hpp"
#include "renderer/opengl/gl_backend.hpp" 
#include "resources/asset_loader.hpp"

ShaderHandle ResourceManager::createShaderProgram(const std::vector<std::string> &filePaths) {

    ShaderProgram program = GLBackend::createShaderProgram();
    
    for (const auto &path : filePaths) {

        std::string shaderSource = AssetLoader::readFile(path);

        if (shaderSource.empty()) {
            std::cerr << "[ERROR]: Skipping shader program " << " due to missing file: " << path << "\n";
            continue;;
        }
        const char* c_shaderSource = shaderSource.c_str();

        std::filesystem::path filePath = path;

        uint32_t shader {};

        if (filePath.extension() == ".vert") {
            shader = GLBackend::compileVertShader(c_shaderSource);
        }
        else if (filePath.extension() == ".frag") {
            shader = GLBackend::compileFragShader(c_shaderSource);
        }
        else {
            std::cerr << "[ERROR]: Unkown shader type " << path << "\n";
            continue;
        }

        GLBackend::attachShader(program, shader);
        GLBackend::deleteShader(shader);
    }

    GLBackend::linkProgram(program);

    ShaderHandle handle = shaderPrograms.size();
    shaderPrograms.push_back(program);

    ShaderAsset shaderAsset;
    shaderAsset.shaderProgram = program;
    shaderAssets.push_back(shaderAsset);

    return handle;
}

ShaderProgram ResourceManager::getShaderProgram(ShaderHandle shaderHandle) {

    assert(!(shaderHandle >= shaderPrograms.size()) && "[ERROR]: Shader program does not exist");

    return shaderPrograms[shaderHandle];
}

ShaderAsset& ResourceManager::getShaderAsset(ShaderHandle shaderHandle) {
    assert(!(shaderHandle >= shaderPrograms.size()) && "[ERROR]: Shader program does not exist");
    return shaderAssets[shaderHandle];
}

void ResourceManager::setUniformLocation(ShaderHandle shaderHandle, const std::string& uniformName) {

    ShaderAsset& shaderAsset =  getShaderAsset(shaderHandle);

    if (shaderAsset.uniformLocations.find(uniformName) == shaderAsset.uniformLocations.end()) {
        GLuint location = GLBackend::getUniformLocation(shaderAsset.shaderProgram, uniformName);
        std::cerr << "[DEBUG]: Uniform name: " << uniformName << ", location: " << location << "\n";
        shaderAsset.uniformLocations[uniformName] = location;
    }
}

void ResourceManager::insertMeshAsset(MeshHandle meshHandle, const MeshAsset& meshAsset) {

    if (meshHandle >= meshAssets.size()) {
        meshAssets.resize(meshAssets.size() + 1);
    }
    meshAssets[meshHandle] = meshAsset;
}

MeshAsset& ResourceManager::getMeshAsset(MeshHandle meshHandle) {

    assert(!(meshHandle >= meshAssets.size()) && "Error: MeshAsset does not exist");
    return meshAssets[meshHandle];
}   

GPUMesh ResourceManager::buildGPUMesh(const MeshData& meshData) {

    GPUMesh gpuMesh;
    GLBackend::createVertexArray(gpuMesh.vao);
    GLBackend::createBuffer(gpuMesh.vbo);
    GLBackend::createBuffer(gpuMesh.ebo);
    gpuMesh.indexCount = meshData.indices.size();

    GLBackend::uploadBuffer(gpuMesh.vbo, meshData.vertices.size() * sizeof(Vertex), meshData.vertices.data());
    GLBackend::uploadBuffer(gpuMesh.ebo, meshData.indices.size() * sizeof(uint32_t), meshData.indices.data());

    GLBackend::attachVertexBuffer(gpuMesh.vao, meshData.vertexLayout.bindingIndex, gpuMesh.vbo, 0, meshData.vertexLayout.stride);
    GLBackend::attachElementBuffer(gpuMesh.vao, gpuMesh.ebo);

    for (const VertexAttribute& attribute : meshData.vertexLayout.attributes) {
        GLBackend::setAttribute(gpuMesh.vao, meshData.vertexLayout.bindingIndex, attribute);
    }

    return gpuMesh;
}

GPUMesh& ResourceManager::getGPUMesh(MeshHandle meshHandle) {

    assert(!(meshHandle >= meshAssets.size()) && "Error: GPUMesh does not exist");

    return meshAssets[meshHandle].gpuMesh; 
}

Mesh ResourceManager::loadMesh(const MeshData& meshData, bool storeMeshData) {

    MeshAsset meshAsset; 
    meshAsset.gpuMesh = buildGPUMesh(meshData);

    if (storeMeshData) {
        meshAsset.meshData = meshData;
    }

    const MeshHandle meshHandle = meshAssets.size();
    meshAssets.push_back(meshAsset);

    Mesh mesh = {
        .handle = meshHandle
    };

    return mesh;
};

TextureHandle ResourceManager::loadTexture(const std::string& filePath) {

    ImageData imageData = AssetLoader::loadImage(filePath);

    Texture texture;
    GLBackend::createTexture2D(texture.id);
    GLBackend::allocateTexture2D(texture.id, imageData.format, imageData.width, imageData.height);
    GLBackend::uploadTexture2D(texture.id, imageData.format, imageData.width, imageData.height, imageData.data);

    AssetLoader::freeImage(imageData.data);

    TextureHandle handle = textures.size();
    textures.push_back(texture);

    return handle;
}

Texture& ResourceManager::getTexture(TextureHandle textureHandle) {
    
    assert(!(textureHandle >= textures.size()) && "Error: Texture does not exist");

    return textures[textureHandle]; 
}

void ResourceManager::setTex2DParameters(TextureHandle textureHandle, const Tex2DParameters& parameters) {

    Texture& texture = getTexture(textureHandle);

    GLBackend::setTexture2DWrap(texture.id, parameters.wrapS,  parameters.wrapT);

    auto isMipmapFilter = [](TexFilter f) {
        return f == TexFilter::LinearMipmapLinear ||
               f == TexFilter::LinearMipmapNearest ||
               f == TexFilter::NearestMipmapNearest ||
               f == TexFilter::NearestMipmapLinear;
    };

    if (parameters.enableMipmap) {

        GLBackend::generateMipmap(texture.id);

        if (!isMipmapFilter(parameters.minFilter)) {
            std::cerr << "[ERROR]: Min filter is not a mipmap filter but mipmaps are enabled \n"; 
            return;
        }
        GLBackend::setTexture2DFilter(texture.id, parameters.minFilter, parameters.magFilter);
    }
    else {

        if (isMipmapFilter(parameters.minFilter)) {
            std::cerr << "[ERROR]: Mipmap filter provided but mipmaps are disabled \n"; 
            return;
        }
        GLBackend::setTexture2DFilter(texture.id, parameters.minFilter, parameters.magFilter);
    }
}

Model ResourceManager::loadModel(const std::string& filePath) {
    Model model;

    const ModelImport& import = AssetLoader::loadModel(filePath);

    std::vector<MaterialHandle> materialHandles;
    materialHandles.reserve(import.materialImports.size());

    for (const auto& materialImport : import.materialImports) {

        MaterialAsset materialAsset;

        for (const auto& textureImport : materialImport.textureImports) {
    
            TextureHandle texHandle = loadTexture(textureImport.path);
    
            switch (textureImport.type) {
                case TexType::DiffuseMap:
                    materialAsset.diffuseMap = texHandle;
                    break;
                case TexType::SpecularMap:
                    materialAsset.specularMap = texHandle;
                    break;
                case TexType::Unknown:
                    std::cerr << "[ERROR]: Unknown texture type provided\n";
                    break;
                default:
                    break;
            }
        }
        const MaterialHandle materialHandle = materialAssets.size();
        materialAssets.push_back(materialAsset);
        materialHandles.push_back(materialHandle);

    }

    for (const auto& meshImport : import.meshImports) {

        const MaterialHandle materialHandle = materialHandles[meshImport.materialIndex];

        const MeshData& meshData = meshImport.meshData;
    
        const Mesh mesh = loadMesh(meshData);

        Model::Part part = {
            .meshHandle = mesh.handle,
            .materialHandle = materialHandle
        };

        model.parts.emplace_back(part);
    }

    return model;
}

void ResourceManager::destroy() {

    for (ShaderProgram& program : shaderPrograms) {
        GLBackend::deleteShaderProgram(program);
    }

    for (MeshAsset& meshAsset  : meshAssets) {
        GLBackend::destroyMesh(meshAsset.gpuMesh);
    }

    for (Texture& texture : textures) {
        GLBackend::deleteTexture(texture.id);
    }

    shaderPrograms.clear();
    shaderAssets.clear();
    textures.clear();
    meshAssets.clear();
}
