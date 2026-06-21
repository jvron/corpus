#include <GLFW/glfw3.h>
#include <cstring>

#include "platform/input.hpp"
#include "engine/world.hpp"

void Input::init(World& world) {
    
    world.engineState.inputState.lastMouseX = (float) world.engineConfig.windowConfig.width / 2.0f;
    world.engineState.inputState.lastMouseY = (float) world.engineConfig.windowConfig.height / 2.0f; 

    glfwSetKeyCallback(world.engineState.windowState.handle, keyCallback);
    glfwSetCursorPosCallback(world.engineState.windowState.handle, cursorCallback);
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods ) {

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

void Input::cursorCallback(GLFWwindow* window, double xPos, double yPos) {

    World* worldHandle = static_cast<World*>(glfwGetWindowUserPointer(window));
    if (!worldHandle) {
        return;
    }

    World& world = *worldHandle;
    InputState& inputState = world.engineState.inputState;
    
    if (inputState.firstMouse) {
        inputState.lastMouseX = xPos;
        inputState.lastMouseY = yPos;
        inputState.firstMouse = false;
    }

    inputState.mouseDeltaX = xPos - inputState.lastMouseX;
    inputState.mouseDeltaY = inputState.lastMouseY - yPos;

    inputState.lastMouseX = xPos;
    inputState.lastMouseY = yPos;
}

void Input::resetKeyStates(World& world) {
    
    InputState &inputState = world.engineState.inputState;
    
    memset(inputState.keyPressed, 0, sizeof(inputState.keyPressed));
    memset(inputState.keyReleased, 0, sizeof(inputState.keyReleased));
}

bool Input::isKeyPressed(World& world, Key key) {
    
    return world.engineState.inputState.keyPressed[(int)key];
}

bool Input::isKeyReleased(World& world, Key key) {

    return world.engineState.inputState.keyReleased[(int)key];
}

bool Input::isKeyDown(World& world, Key key) {

    return world.engineState.inputState.keyDown[(int)key];
}

void Input::enableCursor(World& world) {
    glfwSetInputMode(world.engineState.windowState.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Input::disableCursor(World& world) {
    glfwSetInputMode(world.engineState.windowState.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

