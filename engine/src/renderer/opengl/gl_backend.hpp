#pragma once
#include <cstddef>
#include <glad/glad.h>


#include "ecs/components.hpp"

namespace GLBackend {

    void clearBuffer(Color &clearColor);

    void createBuffer(GLuint &buffer); //alocate buffer on the GPU
    void uploadBuffer(GLuint buffer, size_t size, const void* data); // upload data to the allocated GPU buffer
    
    void createVertexArray(GLuint &vao);
    void AttachVertexBuffer(GLuint vao, GLuint buffer, size_t stride);
    void setAttribute(GLuint vao, size_t offset);
}