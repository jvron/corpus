#pragma once
#include <vector>

#include "components.hpp"

constexpr int32_t keyCount = 348; 


struct WindowConfig { // user defined
    int width;
    int height;
    const char* title;
};

struct WindowState {
    void* handle;
};

struct InputState {
    
    // per frame key state (reset per frame)
    bool keyPressed[keyCount] {};
    bool keyReleased[keyCount] {};

    // overall key state
    bool keyState[keyCount] {};
};

class Registry {

    public:
    WindowConfig windowConfig;
    WindowState windowState;
    InputState inputState;

    void addComponent(Component Component);
    std::vector<Component> components; 
};