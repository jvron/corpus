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

// vertex array
void GLBackend::createVertexArray(GLuint &vao) {
    glCreateVertexArrays(1, &vao);
}

void GLBackend::attachVertexBuffer(GLuint vao, GLuint buffer, size_t size) {
    glVertexArrayVertexBuffer(vao, 0, buffer, 0, size);
}

void GLBackend::setAttribute(GLuint vao, size_t offset) {
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offset);
    glVertexArrayAttribBinding(vao, 0, 0);
}

//shaders
GLuint GLBackend::compileVertShader(const char *shaderSource) {

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderSource, NULL);
    glCompileShader(vertexShader);
    return vertexShader;
}

GLuint GLBackend::compileFragShader(const char *shaderSource) {
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderSource, NULL);
    glCompileShader(fragmentShader);
    return fragmentShader;
}

GLuint GLBackend::createShaderProgram() {
    return glCreateProgram();
}

void GLBackend::attachShader(GLuint &shaderProgram, GLuint &shader) {
    glAttachShader(shaderProgram, shader);
}

void GLBackend::deleteShader(GLuint &shader) {
    glDeleteShader(shader);
}

void GLBackend::linkProgram(GLuint &shaderProgram) {
    glLinkProgram(shaderProgram);
}