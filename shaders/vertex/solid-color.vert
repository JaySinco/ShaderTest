#version 430 core

layout(location = 0) in vec3 vt_Pos;

uniform mat4 uf_ModelViewProjectionMatrix;

void main() { gl_Position = uf_ModelViewProjectionMatrix * vec4(vt_Pos, 1.0f); }
