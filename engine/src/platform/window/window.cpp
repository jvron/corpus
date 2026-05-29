#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "window.hpp"

void Window::create(int width, int height, const char* title) {

    if (!glfwInit()) {

        std::cerr << "glfw init failed \n";
        return;
    }
    // configuring OpenGL context for the window to be created
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, title, NULL,  NULL);
    if (!handle) {
        glfwTerminate();
        std::cerr << "glfw window creation failed \n";
        return;
    }

    glfwMakeContextCurrent(handle); // assign the window's openGL context to the TLS of the current thread
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(handle);
}

void Window::pollEvents() {
    glfwPollEvents();
}
void Window::swapBuffers() {
    glfwSwapBuffers(handle);
}

void* Window::getHandle() {
    return handle;
}

void Window::destroy() {
    glfwDestroyWindow(handle);
    glfwTerminate();
}