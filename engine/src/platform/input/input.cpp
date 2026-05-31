#include "engine/input.hpp"
#include "GLFW/glfw3.h"
#include "engine/registry.hpp"
#include <cstring>

void Input::init(Registry &registry) {
     
    glfwSetKeyCallback(registry.windowState.handle, keyCallBack);
}

void Input::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods ) {

    Registry* registryHandle = static_cast<Registry*>(glfwGetWindowUserPointer(window)); // get registry pointer from window
    
    if (!registryHandle) {
        return;
    } 
    switch (action) {
        case GLFW_PRESS:
            registryHandle->inputState.keyPressed[key] = true;
            registryHandle->inputState.keyDown[key] = true;
            break;
        case GLFW_RELEASE:
            registryHandle->inputState.keyReleased[key] = true;
            registryHandle->inputState.keyDown[key] = false;
            break;
    }
}

void Input::resetKeyStates(Registry &registry) {
    
    InputState &inputState = registry.inputState;
    
    memset(inputState.keyPressed, 0, sizeof(inputState.keyPressed));
    memset(inputState.keyReleased, 0, sizeof(inputState.keyReleased));
}

bool Input::isKeyPressed(Registry &registry, Key key) {
    
    return registry.inputState.keyPressed[(int)key];
}

bool Input::isKeyReleased(Registry &registry, Key key) {

    return registry.inputState.keyReleased[(int)key];
}

bool Input::isKeyDown(Registry &registry, Key key) {

    return registry.inputState.keyDown[(int)key];
}


