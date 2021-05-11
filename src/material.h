#pragma once
#include "shader.h"
#include "texture.h"
#include <vector>
#include <memory>

namespace gl
{
class Material
{
public:
    Material(float specular = 0.5f, float shineness = 32);
    void attach(std::shared_ptr<Texture> texture);
    void use(Shader &shader) const;

private:
    std::vector<std::shared_ptr<Texture>> textures;
    glm::vec3 specular;
    float shininess;
};

}  // namespace gl
