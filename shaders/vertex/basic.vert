#version 430 core

layout(location = 0) in vec3 vt_Pos;
layout(location = 1) in vec3 vt_Normal;
layout(location = 2) in vec2 vt_TexCoord;

out vec2 io_TexCoord;

uniform mat4 uf_ModelMatrix;
uniform mat4 uf_ViewMatrix;
uniform mat4 uf_ProjectionMatrix;

void main()
{
    gl_Position = uf_ProjectionMatrix * uf_ViewMatrix * uf_ModelMatrix * vec4(vt_Pos, 1.0f);
    io_TexCoord = vt_TexCoord;
}
