#pragma once

#include <cstdint>

//#include "engine/world.hpp"

struct World;
struct GLFWwindow; 

enum class Key : int32_t {

    SPACE         = 32, 
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
    void init(World &world);
    void resetKeyStates(World &world);
    void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

    // game systems
    bool isKeyPressed(World &world, Key key);
    bool isKeyReleased(World &world, Key key);
    bool isKeyDown(World &world, Key key);

    void enableCursor(World& world);
    void disableCursor(World &world);
}