#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "window.hpp"
#include "engine/registry.hpp"

void Window::create(Registry &registry) {

    WindowConfig config = registry.windowConfig;

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

    registry.windowState.handle = handle;
}

bool Window::shouldClose(Registry &registry) {
    return glfwWindowShouldClose(registry.windowState.handle);
}

void Window::pollEvents(Registry &registry) {
    glfwPollEvents();
}
void Window::swapBuffers(Registry &registry) {
    glfwSwapBuffers(registry.windowState.handle);
}

void Window::destroy(Registry &registry) {
    glfwDestroyWindow(registry.windowState.handle);
    glfwTerminate();
}