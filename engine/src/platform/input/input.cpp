#include "engine/input.hpp"
#include "GLFW/glfw3.h"
#include "engine/registry.hpp"
#include <cstring>

void Input::init(Registry &registry) {
    registryHandle = &registry; 
    glfwSetKeyCallback((GLFWwindow*) registry.windowState.handle, keyCallBack);
}

void Input::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods ) {

    if (!registryHandle) {
        return;
    } 
    switch (action) {
        case GLFW_PRESS:
            registryHandle->inputState.keyPressed[key] = true;
        case GLFW_RELEASE:
            registryHandle->inputState.keyReleased[key] = true;
    }

}

bool Input::keyIsPressed(Key key) {

    return registryHandle->inputState.keyPressed[(int)key];
}

void Input::resetKeyStates(Registry &registry) {

    memset(registryHandle->inputState.keyPressed, 0, sizeof(registryHandle->inputState.keyPressed));
    memset(registryHandle->inputState.keyReleased, 0, sizeof(registryHandle->inputState.keyPressed));
}