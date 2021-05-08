#pragma once
#include "shader.h"
#include <string>

namespace gl
{
class Texture
{
public:
    Texture() = default;
    Texture(const Texture &) = delete;
    ~Texture();
    bool load(const std::wstring &imageFile, bool flip_vertically = false);
    void use(Shader &shader, unsigned idx) const;

private:
    bool loaded = false;
    unsigned texture;
};

}  // namespace gl
