#pragma once

#include "GLFW/glfw3.h"
class Window {

public:
    void create(int width, int height, const char* title);
    bool shouldClose();
    void pollEvents();
    void swapBuffers();
    void* getHandle();
    void destroy();

private:
    GLFWwindow* handle; // *GLFWwindow

};