#version 430 core
#define MAX_LIGHTS 8

in vec3 io_Pos;
in vec3 io_Normal;
in vec2 io_TexCoord;

out vec4 frag_Color;

uniform sampler2D uf_Texture0;

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
    vec3 specular;
    float shininess;
} uf_Material;

vec3 shineAmbient(Light light) { return light.color; }

vec3 shineDirect(Light light) { return vec4(0); }

vec3 shinePoint(Light light)
{
    vec3 normal = normalize(io_Normal);
    vec3 lightDir = normalize(light.position - io_Pos);
    float diffuse = max(dot(normal, lightDir), 0.0);

    vec3 viewDir = normalize(vec3(0) - io_Pos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uf_Material.shininess);

    return light.color * (diffuse + spec * uf_Material.specular);
}

vec3 shineSpot(Light light) { return vec4(0); }

void main()
{
    vec3 blendLight = vec3(0);
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        switch (uf_Lights[i].type) {
            case 0:
                break;
            case 1:
                blendLight += shineAmbient(uf_Lights[i]);
                break;
            case 2:
                blendLight += shineDirect(uf_Lights[i]);
                break;
            case 3:
                blendLight += shinePoint(uf_Lights[i]);
                break;
            case 4:
                blendLight += shineSpot(uf_Lights[i]);
                break;
        }
    }
    vec4 skyBlue = vec4(vec3(135, 206, 235) / 255, 1);
    vec4 texColor = texture(uf_Texture0, io_TexCoord);
    frag_Color = vec4(blendLight, 1.0) * skyBlue;
}
