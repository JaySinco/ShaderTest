#pragma once
#include <glm/mat4x4.hpp>

namespace gl
{
class Mesh
{
public:
    Mesh();
    ~Mesh();
    bool load(size_t vtLen, float *vtData, size_t idLen, unsigned *idData);
    void draw() const;
    void reset();
    void move(float dx, float dy, float dz);
    void moveTo(float x, float y, float z);
    void spin(float ddegree, float axis_x, float axis_y, float axis_z);
    void spinTo(float degree, float axis_x, float axis_y, float axis_z);
    void zoom(float dx, float dy, float dz);
    void zoomTo(float x, float y, float z);
    glm::mat4 getModelMatrix() const;

private:
    glm::mat4 translate, rotate, scale;
    unsigned vao, vbo, ebo;
    unsigned indices;
};

}  // namespace gl
