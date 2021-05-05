#include "mesh.h"
#include <glad/glad.h>

namespace gl
{
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->ebo);
}

bool Mesh::load(size_t vtLen, float *vtData, size_t idLen, unsigned *idData)
{
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vtLen * sizeof(float), vtData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idLen * sizeof(unsigned), idData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    this->indices = idLen;
    return true;
}

bool Mesh::draw()
{
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->indices, GL_UNSIGNED_INT, 0);
    return true;
}

}  // namespace gl
