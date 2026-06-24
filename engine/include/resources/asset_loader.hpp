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

    std::string readFile(const std::string& filePath);
    ImageData loadTexture(const std::string& filePath);
    void freeData(void* data);
}