#pragma once

struct ImageData {
    int width {};
    int height {};
    unsigned char* data = nullptr;
};

namespace AssetLoader  {
    ImageData loadTexture(const std::string& filePath);
}