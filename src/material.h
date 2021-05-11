#pragma once
#include "shader.h"
#include <glm/vec3.hpp>

namespace gl
{
class Material
{
public:
    Material(float specular = 0.5f, float shineness = 32);
    void use(Shader &shader) const;

private:
    glm::vec3 specular;
    float shininess;
};

}  // namespace gl
