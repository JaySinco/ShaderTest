#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace gl
{
struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh
{
public:
    ~Mesh();
    void load(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices);
    void draw() const;

private:
    unsigned vao, vbo, ebo;
    unsigned indices;
};

}  // namespace gl
