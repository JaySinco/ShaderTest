#include "shader.h"
#include "utils.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <filesystem>

namespace gl
{
Shader::~Shader()
{
    if (this->loaded) {
        glDeleteProgram(this->program);
    }
}

bool Shader::load(const std::wstring &vertFile, const std::wstring &fragFile)
{
    auto vertRes = Shader::compile(vertFile);
    auto [vertOk, vertShader] = vertRes;
    if (!vertOk) {
        return false;
    }
    std::shared_ptr<void> vertShader_guard(nullptr,
                                           [=](void *) { glDeleteShader(vertRes.second); });

    auto fragRes = Shader::compile(fragFile);
    auto [fragOk, fragShader] = fragRes;
    if (!fragOk) {
        return false;
    }
    std::shared_ptr<void> fragShader_guard(nullptr,
                                           [=](void *) { glDeleteShader(fragRes.second); });

    this->program = glCreateProgram();
    glAttachShader(this->program, vertShader);
    glAttachShader(this->program, fragShader);
    glLinkProgram(this->program);
    int succ = 0;
    glGetProgramiv(this->program, GL_LINK_STATUS, &succ);
    if (!succ) {
        char log[512]{0};
        glGetProgramInfoLog(this->program, 512, NULL, log);
        LOG(ERROR) << "failed to link shader program:\n" << log;
        return false;
    }

    this->loaded = true;
    return true;
}

void Shader::use() const { glUseProgram(this->program); }

std::pair<bool, unsigned> Shader::compile(const std::wstring &file)
{
    unsigned type;
    std::wstring suffix = std::filesystem::path(file).extension().generic_wstring();
    if (suffix == L".vert") {
        type = GL_VERTEX_SHADER;
    } else if (suffix == L".frag") {
        type = GL_FRAGMENT_SHADER;
    } else {
        LOG(ERROR) << "invalid shader file extension: " << utils::ws2s(suffix);
        return {false, 0};
    }
    unsigned shader = glCreateShader(type);
    auto [ok, source] = utils::read_file(file);
    const char *pSource = source.c_str();
    glShaderSource(shader, 1, &pSource, nullptr);
    glCompileShader(shader);
    int succ = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        char log[512]{0};
        glGetShaderInfoLog(shader, 512, NULL, log);
        LOG(ERROR) << "failed to compile shader: " << utils::ws2s(file) << "\n" << log;
        return {false, 0};
    }
    return {true, shader};
}

int Shader::get_loc(const std::string &name) const
{
    this->use();
    return glGetUniformLocation(this->program, name.c_str());
}

void Shader::set(const std::string &name, int v) const { glUniform1i(this->get_loc(name), v); }

void Shader::set(const std::string &name, float v) const { glUniform1f(this->get_loc(name), v); }

void Shader::set(const std::string &name, const glm::vec2 &v) const
{
    glUniform2fv(this->get_loc(name), 1, glm::value_ptr(v));
}

void Shader::set(const std::string &name, const glm::vec3 &v) const
{
    glUniform3fv(this->get_loc(name), 1, glm::value_ptr(v));
}

void Shader::set(const std::string &name, const glm::vec4 &v) const
{
    glUniform4fv(this->get_loc(name), 1, glm::value_ptr(v));
}

void Shader::set(const std::string &name, const glm::mat3 &v) const
{
    glUniformMatrix3fv(this->get_loc(name), 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::set(const std::string &name, const glm::mat4 &v) const
{
    glUniformMatrix4fv(this->get_loc(name), 1, GL_FALSE, glm::value_ptr(v));
}

}  // namespace gl
