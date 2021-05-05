#version 330 core

layout(location = 0) in vec3 vt_Pos;
layout(location = 1) in vec3 vt_Color;

out vec3 io_Color;

void main()
{
    io_Color = vt_Color;
    gl_Position = vec4(vt_Pos.xyz, 1.0);
}
