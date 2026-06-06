#pragma once

#include "ecs/components.hpp"
#include "ecs/sparse_set.hpp"


constexpr int32_t keyCount = 348; 
struct GLFWwindow;

struct WindowConfig { // user defined
    int width {};
    int height {};
    const char* title = nullptr;
};

struct WindowState {
    GLFWwindow* handle = nullptr;
};

struct InputState {
    
    // per frame key state (reset per frame)
    bool keyPressed[keyCount] {};
    bool keyReleased[keyCount] {};

   // persistent key state 
    bool keyDown[keyCount] {};
};

struct RenderState {
    Color clearColor;
};

struct Resources {

    WindowConfig windowConfig;
    WindowState windowState;
    InputState inputState;
    RenderState renderState;
};

class Registry {
public:

    SparseSet<Transform> transformPool;
    SparseSet<Mesh> meshPool;
    SparseSet<Renderable> renderablePool;

    template<typename T> 
    auto& getPool() {
        if constexpr (std::is_same_v<T, Transform>) { // constexpr if statements run during compile time
            return transformPool;
        }
        else if constexpr (std::is_same_v<T, Mesh>) {
            return meshPool;
        }
        else if constexpr (std::is_same_v<T, Renderable>) {
            return renderablePool;
        }
    }
};