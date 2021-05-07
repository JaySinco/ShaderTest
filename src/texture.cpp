#include "texture.h"
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

namespace gl
{
Texture::~Texture() { glDeleteTextures(1, &this->texture); }

bool Texture::load(const std::wstring &imageFile)
{
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, channel;
    stbi_set_flip_vertically_on_load(true);
    std::string path = utils::ws2s(imageFile);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channel, 0);
    if (data == nullptr) {
        LOG(ERROR) << "failed to load image: " << path;
        return false;
    }
    if (channel != 3 && channel != 4) {
        LOG(ERROR) << "invalid image channel: " << channel;
        return false;
    }
    int format = channel == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return true;
}

void Texture::use(unsigned idx) const
{
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

}  // namespace gl
