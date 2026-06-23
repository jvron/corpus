#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

#include "resources/asset_loader.hpp"

ImageData AssetLoader::loadTexture(const std::string& filePath) {

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
            imageData.format = TexFormat::RED;
            break;
        case 2:
            imageData.format = TexFormat::RG;
            break;
        case 3:
            imageData.format = TexFormat::RGB;
            break;
        case 4:
            imageData.format = TexFormat::RGBA;
            break;
        default:
            std::cerr << "[ERROR]: Unsupported number of channels: " << channels << "\n";
            break;
    }

    return imageData;
} 

void AssetLoader::freeData(void* data) {
    stbi_image_free(data);
}