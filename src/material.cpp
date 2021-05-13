#include "material.h"

namespace gl
{
void Material::use(Shader &shader) const
{
    if (this->diffuse) this->diffuse->use(shader, "uf_Material.diffuse", 0);
    if (this->specular) this->specular->use(shader, "uf_Material.specular", 1);
    shader.set("uf_Material.shininess", this->shininess);
}

}  // namespace gl
