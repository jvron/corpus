#include <cstdint>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <cstddef>
#include <iostream>

#include "gl_backend.hpp"
#include "engine/world.hpp"
#include "resources/resource_manager.hpp"

void GLBackend::clearBuffer(Color& clearColor) {
    glClearColor(clearColor.value.r, clearColor.value.g, clearColor.value.b, clearColor.value.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLBackend::createBuffer(GLuint& buffer) {
    glCreateBuffers(1, &buffer);
}

void GLBackend::uploadBuffer(GLuint buffer, size_t size , const void* data) {
    glNamedBufferStorage(buffer, size, data, 0); //immutable buffer
}

// vertex array
void GLBackend::createVertexArray(GLuint& vao) {
    glCreateVertexArrays(1, &vao);
}

void GLBackend::attachVertexBuffer(GLuint vao, uint32_t bindingIndex, GLuint buffer, size_t offset, size_t stride) {
    glVertexArrayVertexBuffer(vao, bindingIndex, buffer, offset, stride); // // attach VBO to binding slot n with given stride
}

void GLBackend::attachElementBuffer(GLuint vao, GLuint buffer) {
    glVertexArrayElementBuffer(vao, buffer);
}

void GLBackend::setAttribute(GLuint vao, uint32_t bindingIndex,const VertexAttribute &attribute) { 

    glEnableVertexArrayAttrib(vao, attribute.location); // enable attribute layout location slot at specified location 
    // set vertex attribute layout at the location
    glVertexArrayAttribFormat(vao, attribute.location, attribute.componentCount, attribute.componentType, attribute.normalized, attribute.relativeOffset); 
    // read from VBO at binding index n
    glVertexArrayAttribBinding(vao, attribute.location, bindingIndex); 
}

//shader

void GLBackend::checkCompileStatus(GLuint shader, GLenum shaderType) {
    int success {};
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        if (shaderType == GL_VERTEX_SHADER) {
            std::cerr << "[ERROR]: Vertex shader compilation failed. \n";
            std::cerr << "InfoLog: " << infoLog << "\n";
        }
        else if (shaderType == GL_FRAGMENT_SHADER) {
            std::cerr << "[ERROR]: Fragment shader compilation failed. \n";
            std::cerr << "InfoLog: " << infoLog << "\n";
        }
    }
}

GLuint GLBackend::compileVertShader(const char* shaderSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderSource, NULL);
    glCompileShader(vertexShader);

    checkCompileStatus(vertexShader,  GL_VERTEX_SHADER);

    return vertexShader;
}

GLuint GLBackend::compileFragShader(const char* shaderSource) {
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderSource, NULL);
    glCompileShader(fragmentShader);

    checkCompileStatus(fragmentShader, GL_FRAGMENT_SHADER);

    return fragmentShader;
}

GLuint GLBackend::createShaderProgram() {
    return glCreateProgram();
}

void GLBackend::attachShader(GLuint& shaderProgram, GLuint& shader) {
    glAttachShader(shaderProgram, shader);
}

void GLBackend::deleteShader(GLuint& shader) {
    glDeleteShader(shader);
}

void GLBackend::linkProgram(GLuint& shaderProgram) {
    glLinkProgram(shaderProgram);
}

void GLBackend::useProgram(GLuint shaderProgram) {
    glUseProgram(shaderProgram);
}

// uniform

GLuint GLBackend::getUniformLocation(ShaderProgram shaderProgram, const std::string &uniformName) {
    return glGetUniformLocation(shaderProgram, uniformName.c_str());
}

void GLBackend::setUniform(ShaderProgram shaderProgram, GLuint location, const glm::vec4& value) {
    glProgramUniform4fv(shaderProgram, location, 1, glm::value_ptr(value));
}

void GLBackend::setUniform(ShaderProgram shaderProgram, GLuint location, const glm::mat4& value) {
    glProgramUniformMatrix4fv(shaderProgram, location, 1, GL_FALSE, glm::value_ptr(value));
}

// draw

void GLBackend::drawIndexed(const GPUMesh& gpuMesh, GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    glBindVertexArray(gpuMesh.vao);
    glDrawElements(GL_TRIANGLES, gpuMesh.indexCount, GL_UNSIGNED_INT, nullptr);
}

void GLBackend::destroyMesh(GPUMesh& gpuMesh) {
    glDeleteBuffers(1, &gpuMesh.vbo);
    glDeleteBuffers(1, &gpuMesh.ebo);
    glDeleteVertexArrays(1, &gpuMesh.vao);
}

void GLBackend::deleteShaderProgram(ShaderProgram program) {
    glDeleteProgram(program);
}