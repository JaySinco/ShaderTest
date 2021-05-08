#include "texture.h"
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

namespace gl
{
Texture::~Texture()
{
    if (this->loaded) {
        glDeleteTextures(1, &this->texture);
    }
}

bool Texture::load(const std::wstring &imageFile, bool flip_vertically)
{
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, channel;
    stbi_set_flip_vertically_on_load(flip_vertically);
    std::string path = utils::ws2s(imageFile);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channel, 0);
    if (data == nullptr) {
        LOG(ERROR) << "failed to load image: " << path;
        return false;
    }
    std::shared_ptr<void> data_guard(nullptr, [=](void *) { stbi_image_free(data); });
    if (channel != 1 && channel != 3 && channel != 4) {
        LOG(ERROR) << "invalid image channel: " << channel;
        return false;
    }
    int format = channel == 1 ? GL_RED : (channel == 3 ? GL_RGB : GL_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    this->loaded = true;
    return true;
}

void Texture::use(unsigned idx, Shader &shader) const
{
    glActiveTexture(GL_TEXTURE0 + idx);
    shader.set(fmt::format("uf_Texture{}", idx), idx);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

}  // namespace gl
