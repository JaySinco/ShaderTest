#version 430 core
#define MAX_LIGHTS 8

in vec3 io_Pos;
in vec3 io_Normal;
in vec2 io_TexCoord;

out vec4 frag_Color;

uniform struct Light
{
    int type;
    vec3 color;
    vec3 position;
    vec3 direction;
    float spotCutoff;
    float spotExponent;
} uf_Lights[MAX_LIGHTS];

uniform struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
} uf_Material;

vec3 castAmbient(Light light)
{
    return light.color * texture(uf_Material.diffuse, io_TexCoord).rgb;
}

vec3 castDirect(Light light) { return vec4(0); }

vec3 castPoint(Light light)
{
    vec3 normal = normalize(io_Normal);
    vec3 lightDir = normalize(light.position - io_Pos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * texture(uf_Material.diffuse, io_TexCoord).rgb;

    vec3 viewDir = normalize(vec3(0) - io_Pos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uf_Material.shininess);
    vec3 specular = spec * texture(uf_Material.specular, io_TexCoord).rgb;

    return light.color * (diffuse + specular);
}

vec3 castSpot(Light light) { return vec3(0); }

void main()
{
    vec3 blendLight = vec3(0);
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        switch (uf_Lights[i].type) {
            case 0:
                break;
            case 1:
                blendLight += castAmbient(uf_Lights[i]);
                break;
            case 2:
                blendLight += castDirect(uf_Lights[i]);
                break;
            case 3:
                blendLight += castPoint(uf_Lights[i]);
                break;
            case 4:
                blendLight += castSpot(uf_Lights[i]);
                break;
        }
    }
    frag_Color = vec4(blendLight, 1.0);
}
