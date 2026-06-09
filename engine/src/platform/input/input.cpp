#include <cstring>
#include <GLFW/glfw3.h>

#include "engine/world.hpp"
#include "engine/input.hpp"

void Input::init(World &world) {
     
    glfwSetKeyCallback(world.resources.windowState.handle, keyCallBack);
}

void Input::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods ) {

    World* worldHandle = static_cast<World*>(glfwGetWindowUserPointer(window)); // get registry pointer from window
    
    if (!worldHandle) {
        return;
    } 
    switch (action) {
        case GLFW_PRESS:
            worldHandle->resources.inputState.keyPressed[key] = true;
            worldHandle->resources.inputState.keyDown[key] = true;
            break;
        case GLFW_RELEASE:
            worldHandle->resources.inputState.keyReleased[key] = true;
            worldHandle->resources.inputState.keyDown[key] = false;
            break;
    }
}

void Input::resetKeyStates(World &world) {
    
    InputState &inputState = world.resources.inputState;
    
    memset(inputState.keyPressed, 0, sizeof(inputState.keyPressed));
    memset(inputState.keyReleased, 0, sizeof(inputState.keyReleased));
}

bool Input::isKeyPressed(World &world, Key key) {
    
    return world.resources.inputState.keyPressed[(int)key];
}

bool Input::isKeyReleased(World &world, Key key) {

    return world.resources.inputState.keyReleased[(int)key];
}

bool Input::isKeyDown(World &world, Key key) {

    return world.resources.inputState.keyDown[(int)key];
}


