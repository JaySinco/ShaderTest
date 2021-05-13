#pragma once
#include "shader.h"
#include "texture.h"
#include <memory>
#include <glm/vec3.hpp>

namespace gl
{
class Material
{
public:
    void use(Shader &shader) const;
    glm::vec4 color{0.0};
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
    float shininess;
};

}  // namespace gl
