#version 430 core

in vec2 io_TexCoord;
in vec3 io_Normal;

out vec4 frag_Color;

uniform sampler2D uf_Texture0;

void main() { frag_Color = texture(uf_Texture0, io_TexCoord); }
