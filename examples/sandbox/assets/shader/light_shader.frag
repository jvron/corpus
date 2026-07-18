#version 450 core

in vec4 vertColor;

out vec4 fragColor;

uniform vec4 uColor;

void main() 
{  
   fragColor =  vec4(uColor);
}