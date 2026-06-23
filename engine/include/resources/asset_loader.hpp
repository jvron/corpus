#pragma once

enum class TexFormat {
    RED,
    RG,
    RGB,
    RGBA
};

struct ImageData {
    int width {};
    int height {};
    void* data = nullptr;
    TexFormat format {};
};

namespace AssetLoader  {
    ImageData loadTexture(const std::string& filePath);
    void freeData(void* data);
}