#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace gl
{
class Shader
{
public:
    Shader() = default;
    Shader(const Shader &) = delete;
    ~Shader();
    bool load(const std::wstring &vertFile, const std::wstring &fragFile);
    void use() const;
    void set(const std::string &name, int v) const;
    void set(const std::string &name, float v) const;
    void set(const std::string &name, const glm::vec2 &v) const;
    void set(const std::string &name, const glm::vec3 &v) const;
    void set(const std::string &name, const glm::vec4 &v) const;
    void set(const std::string &name, const glm::mat3 &v) const;
    void set(const std::string &name, const glm::mat4 &v) const;

private:
    static std::pair<bool, unsigned> compile(const std::wstring &file);
    int get_loc(const std::string &name) const;
    bool loaded = false;
    unsigned program;
};

}  // namespace gl
