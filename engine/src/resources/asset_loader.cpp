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

    return imageData;
} 

void AssetLoader::freeData(void* data) {
    stbi_image_free(data);
}