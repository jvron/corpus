#pragma once

#include <cstdint>

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
    void init(World& world);
    void resetInputState(World& world);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cursorCallback(GLFWwindow* window, double xPos, double yPos);

    // game systems
    bool isKeyPressed(World& world, Key key);
    bool isKeyReleased(World& world, Key key);
    bool isKeyDown(World& world, Key key);

    void enableCursor(World& world);
    void disableCursor(World& world);
}