#pragma once

struct ImageData {
    int width {};
    int height {};
    void* data = nullptr;
};

namespace AssetLoader  {
    ImageData loadTexture(const std::string& filePath);
    void freeData(void* data);
}