#version 430 core

out vec4 frag_Color;

uniform struct Material
{
    vec4 color;
} uf_Material;

void main() { frag_Color = uf_Material.color; }
