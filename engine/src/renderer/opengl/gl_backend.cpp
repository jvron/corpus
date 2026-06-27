#include <glad/glad.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "gl_backend.hpp"
#include "resources/asset_loader.hpp"
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

void GLBackend::setUniform(ShaderProgram shaderProgram, GLuint location, int value) {
    glProgramUniform1i(shaderProgram, location, value);
}

void GLBackend::setUniform(ShaderProgram shaderProgram, GLuint location, float value) {
    glProgramUniform1f(shaderProgram, location, value);
}

// texture

void GLBackend::createTexture2D(GLuint& texture) {
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
}

void GLBackend::allocateTexture2D(GLuint texture, TexFormat format, int width, int height) {

    if (width <= 0 || height <= 0) {
        return;
    }
    int maxDim = std::max(width, height);    
    int levels = static_cast<int>(std::floor(std::log2(maxDim))) + 1; // calculate number of mip levels 

    GLenum internalFormat = toGLInternalFormat(format);
    glTextureStorage2D(texture, levels, internalFormat, width, height); // allocate buffer
}

void GLBackend::uploadTexture2D(GLuint texture, TexFormat format, int width, int height, void* data) {
    GLenum glFormat = toGLFormat(format);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, glFormat, GL_UNSIGNED_BYTE, data); // copy pixel data into buffer
}

GLenum GLBackend::toGLWrap(TexWrap wrap) {

    switch (wrap) {
        case TexWrap::Repeat:
            return GL_REPEAT;

        case TexWrap::MirroredRepeat:
            return GL_MIRRORED_REPEAT;

        case TexWrap::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
            
        case TexWrap::ClampToBorder:
            return GL_CLAMP_TO_BORDER;
    }
    assert(false && "[ERROR]: Unknown texture warp parameter");
    return GL_REPEAT;
}

GLenum GLBackend::toGLFilter(TexFilter filter) {
    
    switch (filter) {
        case TexFilter::Linear:
            return GL_LINEAR;

        case TexFilter::Nearest:
            return GL_NEAREST;

        case TexFilter::NearestMipmapNearest:
            return GL_NEAREST_MIPMAP_NEAREST;

        case TexFilter::NearestMipmapLinear:
            return GL_NEAREST_MIPMAP_LINEAR;

        case TexFilter::LinearMipmapLinear:
            return GL_LINEAR_MIPMAP_LINEAR;

        case TexFilter::LinearMipmapNearest:
            return GL_LINEAR_MIPMAP_NEAREST;
    }
    assert(false && "[ERROR]: Unknown texture filter parameter");
    return GL_LINEAR;
}

GLenum GLBackend::toGLFormat(TexFormat format) {

    switch (format) {
        case TexFormat::RED:
            return GL_RED;
        case TexFormat::RG:
            return GL_RG;
        case TexFormat::RGB:
            return GL_RGB;
        case TexFormat::RGBA:
            return GL_RGBA;
    }
    assert(false && "[ERROR]: Unsupported texture format");
    return GL_RGBA;
}

GLenum GLBackend::toGLInternalFormat(TexFormat format) {

    switch (format) {
        case TexFormat::RED:
            return GL_R8;
        case TexFormat::RG:
            return GL_RG8;
        case TexFormat::RGB:
            return GL_RGB8;
        case TexFormat::RGBA:
            return GL_RGBA8;
    }
    assert(false && "[ERROR]: Unsupported texture format");
    return GL_RGBA8;
}

void GLBackend::setTexture2DWrap(GLuint texture, TexWrap wrapS, TexWrap wrapT) {

    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, toGLWrap(wrapS));
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, toGLWrap(wrapT));
}

void GLBackend::setTexture2DFilter(GLuint texture, TexFilter minFilter, TexFilter magFilter) {

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, toGLFilter(minFilter));
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, toGLFilter(magFilter));
}

void GLBackend::bindTextureUnit(GLuint texture, GLuint unit) {
    glBindTextureUnit(unit, texture);
}

void GLBackend::generateMipmap(GLuint texture) {
    glGenerateTextureMipmap(texture);
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

    gpuMesh = {};
}

void GLBackend::deleteShaderProgram(ShaderProgram& program) {
    glDeleteProgram(program);

    program = 0;
}

void GLBackend::deleteTexture(GLuint& texture) {
    glDeleteTextures(1, &texture);

    texture = 0;
}