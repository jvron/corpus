#pragma once

#include "engine/registry.hpp"
#include <cstdint>

struct GLFWwindow; // forward declaration

enum class Key : int32_t {

    SPACE = 32, 
    A             = 65,
    D             = 68,
    S             = 83,
    W             = 87,
    
    Escape        = 256,
    Enter         = 257,
    LeftControl   = 341,
    LeftShift     = 340
};


namespace Input {

    void init(Registry &registry);
    bool keyIsPressed(Key key);
    void resetKeyStates(Registry &registry);
    void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    inline Registry *registryHandle = nullptr;
};