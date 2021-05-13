#version 430 core
#define MAX_LIGHTS 8

in vec3 io_Pos;
in vec3 io_Normal;
in vec2 io_TexCoord;

out vec4 frag_Color;

struct AttenateSettings
{
    float constant;
    float linear;
    float quadratic;
};

struct SpotSettings
{
    float cutoff;
    float exponent;
};

uniform struct Light
{
    int type;
    vec4 color;
    vec3 position;
    vec3 direction;
    AttenateSettings attenuation;
    SpotSettings spot;
} uf_Lights[MAX_LIGHTS];

uniform struct Material
{
    vec4 color;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
} uf_Material;

vec4 calcDiffuse(vec3 lightDir)
{
    float diff = max(dot(normalize(io_Normal), lightDir), 0.0);
    return diff * (uf_Material.color == vec4(0)
                       ? vec4(texture(uf_Material.diffuse, io_TexCoord).rgb, 1.0)
                       : uf_Material.color);
}

vec4 calcSpecular(vec3 lightDir)
{
    vec3 viewDir = normalize(vec3(0) - io_Pos);
    vec3 reflectDir = reflect(-lightDir, normalize(io_Normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uf_Material.shininess);
    return spec * (uf_Material.color == vec4(0)
                       ? vec4(texture(uf_Material.specular, io_TexCoord).rgb, 1.0)
                       : vec4(0.5));
}

vec4 castAmbient(Light light)
{
    return light.color * vec4(texture(uf_Material.diffuse, io_TexCoord).rgb, 1.0);
}

vec4 castDirect(Light light)
{
    vec3 lightDir = -normalize(light.direction);
    return light.color * (calcDiffuse(lightDir) + calcSpecular(lightDir));
}

vec4 castPoint(Light light)
{
    vec3 lightDir = normalize(light.position - io_Pos);
    return light.color * (calcDiffuse(lightDir) + calcSpecular(lightDir));
}

vec4 castSpot(Light light) { return vec4(0); }

void main()
{
    vec4 blendLight = vec4(0);
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
    frag_Color = blendLight;
}
