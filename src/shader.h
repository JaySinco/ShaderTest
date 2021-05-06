#pragma once
#include <string>

namespace gl
{
class Shader
{
public:
    ~Shader();
    bool load(const std::wstring &vertFile, const std::wstring &fragFile);
    bool use() const;
    bool set(const std::string &name, int v0) const;
    bool set(const std::string &name, float v0, float v1, float v2) const;
    bool set(const std::string &name, float v0, float v1, float v2, float v3) const;

private:
    static std::pair<bool, unsigned> compile(const std::wstring &file);
    std::pair<bool, int> get_loc(const std::string &name) const;
    unsigned program;
};

}  // namespace gl
