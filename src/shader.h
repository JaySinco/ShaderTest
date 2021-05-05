#pragma once
#include <string>

namespace gl
{
class Shader
{
public:
    ~Shader();
    bool load(const std::wstring &vertFile, const std::wstring &fragFile);
    bool use();

private:
    static std::pair<bool, unsigned> compile(const std::wstring &file);
    bool loaded = false;
    unsigned program;
};

}  // namespace gl
