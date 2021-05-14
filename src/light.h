#pragma once
#include "shader.h"
#include "camera.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace gl
{
class Light
{
public:
    void use(Shader &shader, Camera &camera, unsigned idx) const;
    enum Type
    {
        Empty = 0,
        Ambient = 1,
        Direct = 2,
        Point = 3,
        Spot = 4,
    };
    Type type;
    glm::vec4 color;
    glm::vec3 position;
    glm::vec3 direction;
    struct
    {
        float constant;
        float linear;
        float quadratic;
    } attenuation;
    struct
    {
        float cutoff;
        float outerCutoff;
    } spot;
};

}  // namespace gl
