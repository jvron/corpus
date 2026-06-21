#pragma once
//#include <glad/glad.h>
#include <GL/gl.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <cstddef>

#include "ecs/components.hpp"
#include "engine/world.hpp"
#include "resources/resource_manager.hpp"

struct GPUMesh;

namespace GLBackend {

    void clearBuffer(Color& clearColor);

    void createBuffer(GLuint& buffer); //alocate buffer on the GPU
    void uploadBuffer(GLuint buffer, size_t size, const void* data); // upload data to the allocated GPU buffer
    
    void createVertexArray(GLuint& vao);
    void attachVertexBuffer(GLuint vao, uint32_t bindingIndex, GLuint buffer, size_t offset, size_t stride);
    void attachElementBuffer(GLuint vao, GLuint buffer);
    void setAttribute(GLuint vao, uint32_t bindingIndex, const VertexAttribute& attribute);

    GLuint compileVertShader(const char* shaderSource);
    GLuint compileFragShader(const char* shaderSource);
    void checkCompileStatus(GLuint shader, GLenum shaderType);

    GLuint createShaderProgram();
    void attachShader(GLuint& shaderProgram, GLuint& shader);
    void deleteShader(GLuint& shader);
    void linkProgram(GLuint& shaderProgram);
    void useProgram(GLuint shaderProgram);

    GLuint getUniformLocation(ShaderProgram shaderProgram, const std::string& uniformName);
    void setUniform(ShaderProgram shaderProgram, GLuint location, const glm::vec4& value);
    void setUniform(ShaderProgram shaderProgram, GLuint location, const glm::mat4& value);

    void drawIndexed(const GPUMesh& gpuMesh, GLuint shaderProgram);

    void destroyMesh(GPUMesh& gpuMesh);
    void deleteShaderProgram(ShaderProgram program);
}