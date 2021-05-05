#version 330 core

in vec3 io_Color;

out vec4 frag_Color;

void main() { frag_Color = vec4(io_Color, 1.0); }
