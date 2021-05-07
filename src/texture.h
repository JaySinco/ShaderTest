#pragma once
#include <string>

namespace gl
{
class Texture
{
public:
    ~Texture();
    bool load(const std::wstring &imageFile);
    void use(unsigned idx) const;

private:
    unsigned texture;
};

}  // namespace gl
