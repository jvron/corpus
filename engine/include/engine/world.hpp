#pragma once

#include "resources/resource_manager.hpp"
#include "ecs/registry.hpp"


constexpr int32_t keyCount = 348; 
struct GLFWwindow;
typedef unsigned int GLuint;

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

struct EngineState {

    WindowState windowState;
    InputState inputState;
    RenderState renderState;
};

struct EngineConfig {
    WindowConfig windowConfig;
};


struct World {

    EngineConfig engineConfig;
    EngineState engineState;

    Registry registry;
    ResourceManager resourceManager;
};