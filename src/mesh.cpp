#include "mesh.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

namespace gl
{
Mesh::Mesh() { this->reset(); }

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    this->indices = idLen;
    return true;
}

void Mesh::draw() const
{
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->indices, GL_UNSIGNED_INT, 0);
}

void Mesh::reset()
{
    this->translate = glm::mat4(1.0f);
    this->rotate = glm::mat4(1.0f);
    this->scale = glm::mat4(1.0f);
}

void Mesh::move(float dx, float dy, float dz)
{
    this->translate = glm::translate(this->translate, glm::vec3(dx, dy, dz));
}

void Mesh::moveTo(float x, float y, float z)
{
    this->translate = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Mesh::spin(float ddegree, float axis_x, float axis_y, float axis_z)
{
    this->rotate =
        glm::rotate(this->rotate, glm::radians(ddegree), glm::vec3(axis_x, axis_y, axis_z));
}

void Mesh::spinTo(float degree, float axis_x, float axis_y, float axis_z)
{
    this->rotate =
        glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(axis_x, axis_y, axis_z));
}

void Mesh::zoom(float dx, float dy, float dz)
{
    this->scale = glm::scale(this->scale, glm::vec3(dx, dy, dz));
}

void Mesh::zoomTo(float x, float y, float z)
{
    this->scale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

glm::mat4 Mesh::getModelMatrix() const { return this->translate * this->rotate * this->scale; }

}  // namespace gl
