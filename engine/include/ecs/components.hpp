#pragma once 

#include <cstdint>

using MeshHandle = uint32_t;
using ShaderHandle = uint32_t;

struct Entity {
    int32_t id {};
};

struct Color {
    float r {};
    float g {};
    float b {};
    float a {};
};

struct Position {
    float x {};
    float y {};
    float z {};
};

struct Transform {
    Position position {};
};

struct Mesh {  
    MeshHandle meshHandle {};
};

struct Material {
    ShaderHandle shaderHandle {};
};

struct Renderable {
    bool visible = true;
};
