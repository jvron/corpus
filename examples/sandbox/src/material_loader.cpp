#include "header.hpp"

void loadShader(World& world, MaterialAsset& material)
{
    std::string vertexShader = "examples/sandbox/assets/shader/vertex_shader.vert";
    std::string fragmentShader = "examples/sandbox/assets/shader/fragment_shader.frag";

    std::vector<std::string> filePaths = {
        vertexShader,
        fragmentShader
    };

    material.shaderHandle = world.resourceManager.createShaderProgram(filePaths);

    world.resourceManager.setUniformLocation(material.shaderHandle, "uSpecularStrength");
    world.resourceManager.setUniformLocation(material.shaderHandle, "uShininess");
    world.resourceManager.setUniformLocation(material.shaderHandle, "uColor");
    world.resourceManager.setUniformLocation(material.shaderHandle, "uModel");
}

void loadLightShader(World& world, MaterialAsset& material)
{
    std::string vertexShader = "examples/sandbox/assets/shader/vertex_shader.vert";
    std::string fragmentShader = "examples/sandbox/assets/shader/light_shader.frag";

    std::vector<std::string> filePaths = {
        vertexShader,
        fragmentShader
    };

    material.shaderHandle = world.resourceManager.createShaderProgram(filePaths);

    world.resourceManager.setUniformLocation(material.shaderHandle, "uColor");
    world.resourceManager.setUniformLocation(material.shaderHandle, "uModel");
}

void loadTexture(World& world, MaterialAsset& material)
{
    std::string diffusePath = "examples/sandbox/assets/textures/container2.png";
    std::string specularPath = "examples/sandbox/assets/textures/container2_specular.png";

    material.diffuseMap = world.resourceManager.loadTexture(diffusePath);
    material.specularMap = world.resourceManager.loadTexture(specularPath);

    Tex2DParameters textureParams;

    textureParams.magFilter = TexFilter::Nearest;
    textureParams.minFilter = TexFilter::NearestMipmapNearest;
    textureParams.enableMipmap = true;
    textureParams.wrapS = TexWrap::Repeat;
    textureParams.wrapT = TexWrap::Repeat;

    world.resourceManager.setTex2DParameters(material.diffuseMap, textureParams);
    world.resourceManager.setTex2DParameters(material.specularMap, textureParams);
}
