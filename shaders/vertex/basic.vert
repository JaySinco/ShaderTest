#version 330 core

layout(location = 0) in vec3 vertPos;

void main() { gl_Position = vec4(vertPos.xyz, 1.0); }
