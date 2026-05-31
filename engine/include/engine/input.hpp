#pragma once

#include "engine/registry.hpp"
#include <cstdint>

struct GLFWwindow; 

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
    // engine systems
    void init(Registry &registry);
    void resetKeyStates(Registry &registry);
    void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

    // game systems
    bool isKeyPressed(Registry &registry, Key key);
    bool isKeyReleased(Registry &registry, Key key);
    bool isKeyDown(Registry &registry, Key key);
}