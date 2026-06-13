#include <GLFW/glfw3.h>
#include <cstring>

#include "engine/input.hpp"
#include "engine/world.hpp"

void Input::init(World &world) {
     
    glfwSetKeyCallback(world.engineState.windowState.handle, keyCallBack);
}

void Input::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods ) {

    World* worldHandle = static_cast<World*>(glfwGetWindowUserPointer(window)); // get registry pointer from window
    
    if (!worldHandle) {
        return;
    }

    World &world = *worldHandle;
    switch (action) {
        case GLFW_PRESS:
            world.engineState.inputState.keyPressed[key] = true;
            world.engineState.inputState.keyDown[key] = true;
            break;
        case GLFW_RELEASE:
            world.engineState.inputState.keyReleased[key] = true;
            world.engineState.inputState.keyDown[key] = false;
            break;
    }
}

void Input::resetKeyStates(World &world) {
    
    InputState &inputState = world.engineState.inputState;
    
    memset(inputState.keyPressed, 0, sizeof(inputState.keyPressed));
    memset(inputState.keyReleased, 0, sizeof(inputState.keyReleased));
}

bool Input::isKeyPressed(World &world, Key key) {
    
    return world.engineState.inputState.keyPressed[(int)key];
}

bool Input::isKeyReleased(World &world, Key key) {

    return world.engineState.inputState.keyReleased[(int)key];
}

bool Input::isKeyDown(World &world, Key key) {

    return world.engineState.inputState.keyDown[(int)key];
}


