#include <glad/glad.h>
#include "gl_backend.hpp"

void GLBackend::clearBuffer(Color &clearColor) {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLBackend::createBuffer(GLuint &buffer) {
    glCreateBuffers(1, &buffer);
}

void GLBackend::uploadBuffer(GLuint buffer, size_t size , const void* data) {
    glNamedBufferStorage(buffer, size, data, GL_STATIC_DRAW);
}

void GLBackend::createVertexArray(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
}

void GLBackend::AttachVertexBuffer(GLuint vao, GLuint buffer, size_t size) {
    glVertexArrayVertexBuffer(vao, 0, buffer, 0, size);
}

void GLBackend::setAttribute(GLuint vao, size_t offset) {
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offset);
    glVertexArrayAttribBinding(vao, 0, 0);
}
