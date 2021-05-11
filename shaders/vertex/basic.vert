#version 430 core
#define MAX_LIGHTS 8

layout(location = 0) in vec3 vt_Pos;
layout(location = 1) in vec3 vt_Normal;
layout(location = 2) in vec2 vt_TexCoord;

out vec2 io_TexCoord;
out vec3 io_Normal;

uniform mat4 uf_ModelMatrix;
uniform mat4 uf_ViewMatrix;
uniform mat4 uf_ProjectionMatrix;
uniform mat4 uf_ModelViewMatrix;
uniform mat4 uf_ModelViewProjectionMatrix;
uniform mat3 uf_NormalMatrix;

uniform struct p3d_LightSourceParameters
{
    vec4 color;
    vec4 position;  // View-space position
    vec3 spotDirection;
    float spotCutoff;
    float spotExponent;
} uf_LightSource[MAX_LIGHTS];

uniform struct
{
    vec3 specular;
    float shininess;
} uf_Material;

void main()
{
    gl_Position = uf_ModelViewProjectionMatrix * vec4(vt_Pos, 1.0f);
    io_TexCoord = vt_TexCoord;
    io_Normal = uf_NormalMatrix * vt_Normal;
}
