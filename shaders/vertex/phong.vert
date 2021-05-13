#version 430 core

layout(location = 0) in vec3 vt_Pos;
layout(location = 1) in vec3 vt_Normal;
layout(location = 2) in vec2 vt_TexCoord;

out vec3 io_Pos;
out vec3 io_Normal;
out vec2 io_TexCoord;

uniform mat4 uf_ModelMatrix;
uniform mat4 uf_ViewMatrix;
uniform mat4 uf_ProjectionMatrix;
uniform mat4 uf_ModelViewMatrix;
uniform mat4 uf_ModelViewProjectionMatrix;
uniform mat3 uf_NormalMatrix;

void main()
{
    gl_Position = uf_ModelViewProjectionMatrix * vec4(vt_Pos, 1.0f);
    io_Pos = (uf_ModelViewMatrix * vec4(vt_Pos, 1.0f)).xyz;
    io_Normal = uf_NormalMatrix * vt_Normal;
    io_TexCoord = vt_TexCoord;
}
