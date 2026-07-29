#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "resources/resource_manager.hpp"
#include "ecs/components.hpp"
#include "renderer/renderer.hpp"
#include "renderer/opengl/gl_backend.hpp" 
#include "resources/asset_loader.hpp"

void ResourceManager::init() {

    auto createTexture = [](const ImageData& imageData) {
        Texture texture;

        GLBackend::createTexture2D(texture.id);
        GLBackend::allocateTexture2D(texture.id, imageData.format, imageData.width, imageData.height);
        GLBackend::uploadTexture2D(texture.id, imageData.format, imageData.width, imageData.height, imageData.data);

        return texture;
    };

    if (textures.size() < DefaultTexture::Count) {
        textures.resize(DefaultTexture::Count);
    }

    uint8_t whitePixels[] = {255, 255, 255, 255};
  
    ImageData whiteImageData = {
        .width = 1,
        .height = 1,
        .data = whitePixels,
        .format = TexFormat::Rgba,
    };
    textures[DefaultTexture::White] = createTexture(whiteImageData);

    uint8_t blackPixels[] = {0, 0, 0, 0};

    ImageData blackImageData = {
        .width = 1,
        .height = 1,
        .data = blackPixels,
        .format = TexFormat::Rgba,
    };
    textures[DefaultTexture::Black] = createTexture(blackImageData);

    uint8_t normalPixels[] = {128, 128, 255, 255};

    ImageData normalImageData = {
        .width = 1,
        .height = 1,
        .data = normalPixels,
        .format = TexFormat::Rgba,
    };
    textures[DefaultTexture::Normal] = createTexture(normalImageData);
}

void ResourceManager::initShaderSamplers(ShaderHandle shaderHandle) {

    ShaderAsset& shaderAsset = getShaderAsset(shaderHandle);

    setUniformLocation(shaderHandle, "diffuseMap");
    setUniformLocation(shaderHandle, "specularMap");
    setUniformLocation(shaderHandle, "normalMap");

    GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["diffuseMap"], TextureUnit::DiffuseMap);
    GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["specularMap"], TextureUnit::SpecularMap);
    GLBackend::setUniform(shaderAsset.shaderProgram, shaderAsset.uniformLocations["normalMap"], TextureUnit::NormalMap);
}

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

    initShaderSamplers(handle);

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
        //std::cerr << "[DEBUG]: Uniform name: " << uniformName << ", location: " << location << "\n";
        shaderAsset.uniformLocations[uniformName] = location;
    }
}

TextureHandle ResourceManager::createTexture(const ImageData& imageData) {

    Texture texture;
    GLBackend::createTexture2D(texture.id);
    GLBackend::allocateTexture2D(texture.id, imageData.format, imageData.width, imageData.height);
    GLBackend::uploadTexture2D(texture.id, imageData.format, imageData.width, imageData.height, imageData.data);

    TextureHandle handle = textures.size();
    textures.push_back(texture);

    return handle;
}

TextureHandle ResourceManager::loadTexture(const std::string& filePath) {

    ImageData imageData = AssetLoader::loadImage(filePath);

    TextureHandle handle = createTexture(imageData);
    
    AssetLoader::freeImage(imageData);

    return handle;
}

Texture& ResourceManager::getTexture(TextureHandle textureHandle) {
    
    assert(!(textureHandle >= textures.size()) && "Error: Texture does not exist");

    return textures[textureHandle]; 
}

void ResourceManager::setTex2DParameters(TextureHandle textureHandle, const Tex2DParameters& parameters) {

    const Texture& texture = getTexture(textureHandle);

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

Material ResourceManager::loadMaterial(const MaterialAsset& materialAsset) {

    MaterialHandle handle = materialAssets.size();
    materialAssets.push_back(materialAsset);

    Material material = {
        .handle = handle
    };
    return material;
}

MaterialAsset& ResourceManager::getMaterialAsset(MaterialHandle materialHandle) {
    
    assert(materialHandle < materialAssets.size() && "[ERROR]: MaterialAsset does not exist");

    return materialAssets[materialHandle];
}

MeshAsset& ResourceManager::getMeshAsset(MeshHandle meshHandle) {

    assert(!(meshHandle >= meshAssets.size()) && "[ERROR]: MeshAsset does not exist");
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

    assert(!(meshHandle >= meshAssets.size()) && "[ERROR]: GPUMesh does not exist");

    return meshAssets[meshHandle].gpuMesh; 
}

Mesh ResourceManager::loadMesh(const MeshData& meshData, const std::string& meshName, bool storeMeshData) {

    MeshAsset meshAsset; 
    meshAsset.name = meshName;
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
}

SceneNodeID ResourceManager::processNodeImport(SceneAsset& scene, const NodeImport& node, const std::vector<MaterialHandle>& materialHandles, bool storeMeshData) {

    SceneNode sceneNode;
    sceneNode.name = node.name;

    SceneNodeID nodeID = scene.nodes.size();
    scene.nodes.push_back(sceneNode);

    for (auto& meshImport : node.meshImports) {
        Mesh mesh = loadMesh(meshImport.meshData, meshImport.name, storeMeshData);

        MeshRenderer renderer = {
            .material = materialHandles[meshImport.materialIndex],
            .mesh = mesh.handle,
        };
        scene.nodes[nodeID].renderers.push_back(renderer);
    }

    for (size_t i = 0; i < node.children.size(); i++) {

        SceneNodeID childID = processNodeImport(scene, *node.children[i], materialHandles, storeMeshData);
        scene.nodes[nodeID].children.push_back(childID);
    }

    return nodeID;
}

SceneHandle ResourceManager::loadScene(const std::string& filePath, const SceneImportOptions& options) {
    SceneImport sceneImport = AssetLoader::loadScene(filePath);

    std::vector<MaterialHandle> materialHandles;
    materialHandles.reserve(sceneImport.materialImports.size());

    for (const auto& materialImport : sceneImport.materialImports) {

        MaterialAsset materialAsset;
        materialAsset.name = materialImport.name;
        materialAsset.shaderHandle = options.shader;
        materialAsset.shininess = options.materialShininess;
        materialAsset.specularStrength = options.materialSpecularStrength;

        for (auto& textureImport : materialImport.textureImports) {
            
            if (textureImport.type == TexType::Unknown) {
                std::cerr << "[ERROR]: Unknown texture type provided\n";
                continue;
            }

            TextureHandle texHandle = loadTexture(textureImport.path);
            
            switch (textureImport.type) {
                case TexType::DiffuseMap:
                    setTex2DParameters(texHandle, options.diffuseParameters);
                    materialAsset.diffuseMap = texHandle;
                    break;
                case TexType::SpecularMap:
                    setTex2DParameters(texHandle, options.specularParameters);
                    materialAsset.specularMap = texHandle;
                    break;
                case TexType::NormalMap:
                    setTex2DParameters(texHandle, options.normalParameters);
                    materialAsset.normalMap = texHandle;
                    break;
                default:
                    std::cerr << "[ERROR]: Unknown texture type provided\n";
                    break;
            }
        }

        Material material = loadMaterial(materialAsset);

        materialHandles.push_back(material.handle);
    }

    SceneAsset sceneAsset;
    sceneAsset.name = sceneImport.name;
    sceneAsset.root = processNodeImport(sceneAsset, sceneImport.root, materialHandles, options.storeMeshData);

    SceneHandle sceneHandle = sceneAssets.size();
    sceneAssets.push_back(std::move(sceneAsset));

    return sceneHandle;
}

SceneAsset& ResourceManager::getSceneAsset(SceneHandle sceneHandle) {

    assert(sceneHandle < sceneAssets.size() && "[ERROR]: SceneAsset does not exist");

    return sceneAssets[sceneHandle];
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
    materialAssets.clear();
    sceneAssets.clear();
}
