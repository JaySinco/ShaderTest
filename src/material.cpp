#include "material.h"

namespace gl
{
Material::Material(float specular, float shineness): specular(specular), shininess(shineness) {}

void Material::use(Shader &shader) const
{
    shader.set("uf_Material.specular", this->specular);
    shader.set("uf_Material.shininess", this->shininess);
}

}  // namespace gl
