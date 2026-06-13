#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "window.hpp"

void Window::create(World &world) {

    const WindowConfig &config = world.engineConfig.windowConfig;

    if (!glfwInit()) {

        std::cerr << "glfw init failed \n";
        return;
    }
    // configuring OpenGL context for the window to be created
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* handle = glfwCreateWindow(config.width, config.height, config.title, nullptr,  nullptr);
    if (!handle) {
        glfwTerminate();
        std::cerr << "glfw window creation failed \n";
        return;
    }

    glfwMakeContextCurrent(handle); // assign the window's openGL context to the TLS of the current thread

    glfwSetWindowUserPointer(handle, &world); // store world pointer in the window

    world.engineState.windowState.handle = handle;
}

bool Window::shouldClose(World &world) {
    return glfwWindowShouldClose(world.engineState.windowState.handle);
}

void Window::pollEvents(World &world) {
    glfwPollEvents();
}
void Window::swapBuffers(World &world) {
    glfwSwapBuffers(world.engineState.windowState.handle);
}

void Window::destroy(World &world) {
    glfwDestroyWindow(world.engineState.windowState.handle);
    glfwTerminate();
}