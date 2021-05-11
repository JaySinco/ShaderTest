#pragma once
#include "shader.h"
#include <string>

namespace gl
{
class Light
{
public:
    Light();
    void use(Shader &shader) const;

private:
};

}  // namespace gl
