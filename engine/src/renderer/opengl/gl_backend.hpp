#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <cstddef>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <string>

#include "ecs/components.hpp"
#include "resources/asset_loader.hpp"
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
    void setUniform(ShaderProgram shaderProgram, GLint location, const glm::vec4& value);
    void setUniform(ShaderProgram shaderProgram, GLint location, const glm::vec3& value);
    void setUniform(ShaderProgram shaderProgram, GLint location, const glm::mat4& value);
    void setUniform(ShaderProgram shaderProgram, GLint location, float value);
    void setUniform(ShaderProgram shaderProgram, GLint location, int value);
    void setUniform(ShaderProgram shaderProgram, GLint location, uint32_t value);

    void createUBO(GLuint& ubo, size_t size, GLuint bindingSlot);
    void updateUBO(GLuint& ubo, size_t size, const void* data);

    void createTexture2D(GLuint& texture);  
    void allocateTexture2D(GLuint texture, TexFormat format, int width, int height);
    void uploadTexture2D(GLuint texture, TexFormat format, int width, int height, void* data);

    void setTexture2DWrap(GLuint texture, TexWrap wrapS, TexWrap wrapT);
    void setTexture2DFilter(GLuint texture, TexFilter minFilter, TexFilter magFilter);
    void bindTextureUnit(GLuint texture, GLuint unit);
    void generateMipmap(GLuint texture);

    GLenum toGLWrap(TexWrap wrap);
    GLenum toGLFilter(TexFilter filter);
    GLenum toGLFormat(TexFormat format);
    GLenum toGLInternalFormat(TexFormat format);
    GLenum toGLType(ComponentType type);
    GLboolean toGLBoolean(bool value);

    void drawIndexed(const GPUMesh& gpuMesh, GLuint shaderProgram);

    void destroyMesh(GPUMesh& gpuMesh);
    void deleteShaderProgram(ShaderProgram& program);
    void deleteTexture(GLuint& texture);
}