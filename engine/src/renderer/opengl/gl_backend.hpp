#pragma once

#include <cstdint>
#include <GL/gl.h>
#include <cstddef>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "ecs/components.hpp"

struct GPUMesh;
struct VertexAttribute;

using ShaderProgram = uint32_t;

enum class TexWrap {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

enum class TexFilter {
    Linear,
    Nearest,
};

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

    void createTexture2D(GLuint& texture);
    void allocateTexture2D(GLuint texture, int width, int height);
    void uploadTexture2D(GLuint texture, int width, int height, void* pixels);
    GLenum toGLWrap(TexWrap wrap);
    GLenum toGLFilter(TexFilter filter);
    void setTexture2DWrap(GLuint texture, TexWrap wrapS, TexWrap wrapT);
    void setTexture2DFilter(GLuint texture, TexFilter minFilter, TexFilter magFilter);

    void drawIndexed(const GPUMesh& gpuMesh, GLuint shaderProgram);

    void destroyMesh(GPUMesh& gpuMesh);
    void deleteShaderProgram(ShaderProgram program);
}