#pragma once
#include <string>

namespace gl
{
class Texture
{
public:
    Texture() = default;
    Texture(const Texture &) = delete;
    ~Texture();
    bool load(const std::wstring &imageFile);
    void use(unsigned idx) const;

private:
    bool loaded = false;
    unsigned texture;
};

}  // namespace gl
