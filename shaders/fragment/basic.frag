#version 330 core

in vec3 io_Color;
in vec2 io_TexCoord;

out vec4 frag_Color;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

void main()
{
    frag_Color = mix(texture(u_Texture0, io_TexCoord), texture(u_Texture1, io_TexCoord), 0.2);
}
