#version 330 core

layout(location = 0) in vec3 vt_Pos;
layout(location = 1) in vec3 vt_Color;
layout(location = 2) in vec2 vt_TexCoord;

out vec3 io_Color;
out vec2 io_TexCoord;

uniform mat4 uf_ModelViewProjectionMatrix;

void main()
{
    gl_Position = uf_ModelViewProjectionMatrix * vec4(vt_Pos, 1.0f);
    io_Color = vt_Color;
    io_TexCoord = vt_TexCoord;
}
