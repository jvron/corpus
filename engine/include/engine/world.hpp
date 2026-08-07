#pragma once

#include "ecs/components.hpp"
#include "resources/resource_manager.hpp"
#include "ecs/registry.hpp"
#include "scene/model_spawner.hpp"

constexpr int32_t keyCount = 348; 

struct GLFWwindow;

struct WindowConfig {
    int width {};
    int height {};
    std::string title;
    bool enableVSync {true};
};

struct WindowState {
    GLFWwindow* handle = nullptr;
    float aspect {};
};

struct InputState {
    
    // per frame key state (reset per frame)
    bool keyPressed[keyCount] {};
    bool keyReleased[keyCount] {};
    // persistent key state 
    bool keyDown[keyCount] {};

    bool firstMouse {true};
    float lastMouseX {};
    float lastMouseY {};
    float mouseDeltaX {};
    float mouseDeltaY {};

    bool cursorEnabled {false};
};

struct InputConfig {
    float sensitivity {};
};

struct RenderState {
    Color clearColor;

    uint32_t lightUBO {};
    uint32_t cameraUBO {};
};

struct Time {
    double deltaTime {};
    double currentTime {};
    double previousTime {};
    double elapsedTime {};

    uint32_t frameCount {}; // total number of frames since startup
    
    double fpsTimer {};
    int framesThisSecond {};
};

struct EngineState {
    WindowState windowState;
    InputState inputState;
    RenderState renderState;
    Time time;
};

struct EngineConfig {
    WindowConfig windowConfig;
    InputConfig inputConfig;
};

struct World {

    EngineConfig engineConfig;
    EngineState engineState;

    Registry registry;
    ResourceManager resourceManager;
    ModelSpawner modelSpawner;

    Entity activeCamera;

    World() : modelSpawner(registry, resourceManager) {}
};