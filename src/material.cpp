#include "material.h"

namespace gl
{
Material::Material(float specular, float shineness): specular(specular), shininess(shineness) {}

void Material::attach(std::shared_ptr<Texture> texture) { this->textures.push_back(texture); }

void Material::use(Shader &shader) const
{
    for (int i = 0; i < this->textures.size(); ++i) {
        this->textures.at(i)->use(shader, i);
    }
    shader.set("uf_Material.specular", this->specular);
    shader.set("uf_Material.shininess", this->shininess);
}

}  // namespace gl
