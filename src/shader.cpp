#include "shader.h"
#include "utils.h"
#include <glad/glad.h>
#include <filesystem>

namespace gl
{
Shader::~Shader() { glDeleteProgram(this->program); }

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

    return true;
}

bool Shader::use() const
{
    glUseProgram(this->program);
    return true;
}

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

std::pair<bool, int> Shader::get_loc(const std::string &name) const
{
    int loc = glGetUniformLocation(this->program, name.c_str());
    if (loc == -1) {
        LOG(ERROR) << "failed to get uniform location: " << name;
        return {false, 0};
    }
    return {true, loc};
}

bool Shader::set(const std::string &name, int v0) const
{
    auto [ok, loc] = this->get_loc(name);
    if (!ok) {
        return false;
    }
    glUseProgram(this->program);
    glUniform1i(loc, v0);
    return true;
}

bool Shader::set(const std::string &name, float v0, float v1, float v2) const
{
    auto [ok, loc] = this->get_loc(name);
    if (!ok) {
        return false;
    }
    glUseProgram(this->program);
    glUniform3f(loc, v0, v1, v2);
    return true;
}

bool Shader::set(const std::string &name, float v0, float v1, float v2, float v3) const
{
    auto [ok, loc] = this->get_loc(name);
    if (!ok) {
        return false;
    }
    glUseProgram(this->program);
    glUniform4f(loc, v0, v1, v2, v3);
    return true;
}

}  // namespace gl
