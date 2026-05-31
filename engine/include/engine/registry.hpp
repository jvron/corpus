#pragma once

#include "components.hpp"

constexpr int32_t keyCount = 348; 
struct GLFWwindow;

struct WindowConfig { // user defined
    int width;
    int height;
    const char* title = nullptr;
};

struct WindowState {
    GLFWwindow* handle;
};

struct InputState {
    
    // per frame key state (reset per frame)
    bool keyPressed[keyCount] {};
    bool keyReleased[keyCount] {};

   // persistent key state 
    bool keyDown[keyCount] {};
};

struct Registry {

    WindowConfig windowConfig {};
    WindowState windowState {};
    InputState inputState {};
};