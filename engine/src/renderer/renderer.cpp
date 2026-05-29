#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>

#include "renderer.hpp"

void Renderer::init() {

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "glad load failed \n";
    }
}