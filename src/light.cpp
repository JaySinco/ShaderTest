#include "light.h"
#include "utils.h"
#include <glm/gtc/matrix_inverse.hpp>

namespace gl
{
void Light::use(Shader &shader, Camera &camera, unsigned idx) const
{
    glm::vec3 viewPos = camera.getViewMatrix() * glm::vec4(this->position, 1.0f);
    glm::vec3 viewDirection =
        glm::inverseTranspose(camera.getViewMatrix()) * glm::vec4(this->direction, 1.0f);

    shader.set(fmt::format("uf_Lights[{}].type", idx), static_cast<int>(this->type));
    shader.set(fmt::format("uf_Lights[{}].color", idx), this->color);
    shader.set(fmt::format("uf_Lights[{}].position", idx), viewPos);
    shader.set(fmt::format("uf_Lights[{}].direction", idx), viewDirection);
    shader.set(fmt::format("uf_Lights[{}].spotCutoff", idx), this->spotCutoff);
    shader.set(fmt::format("uf_Lights[{}].spotExponent", idx), this->spotExponent);
}

}  // namespace gl
