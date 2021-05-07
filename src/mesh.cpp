#include "mesh.h"
#include <glad/glad.h>

namespace gl
{
Mesh::~Mesh()
{
    if (this->loaded) {
        glDeleteVertexArrays(1, &this->vao);
        glDeleteBuffers(1, &this->vbo);
        glDeleteBuffers(1, &this->ebo);
    }
}

Mesh::Mesh(Mesh &&rhs)
{
    this->loaded = rhs.loaded;
    this->vao = rhs.vao;
    this->vbo = rhs.vbo;
    this->ebo = rhs.ebo;
    this->indices = rhs.indices;
    rhs.loaded = false;
}

void Mesh::load(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices)
{
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, texCoord));
    glBindVertexArray(0);

    this->indices = indices.size();
    this->loaded = true;
}

void Mesh::draw() const
{
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->indices, GL_UNSIGNED_INT, 0);
}

}  // namespace gl
