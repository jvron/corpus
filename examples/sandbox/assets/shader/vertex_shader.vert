#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;


layout (std140, binding = 1) uniform CameraBlock {
   vec3 position;
   float unused0;
   mat4 view;
   mat4 projection;
} camera;

out vec4 vertColor;
out vec2 texCoord;
out vec3 normal;

out vec3 fragPos;
out vec3 viewPos;

uniform mat4 uModel;
//uniform mat4 uView;
//uniform mat4 uProjection;

void main()
{   
    vec4 pos = vec4(aPos, 1.0);
    gl_Position = camera.projection * camera.view * uModel * pos;

    fragPos = vec3(uModel * pos);

    normal = mat3(transpose(inverse(uModel))) * aNormal;

    vertColor = aColor;
    texCoord = aTexCoord;
    viewPos = camera.position;
}