#pragma once
#include "mesh.h"
#include <vector>
#include <string>
#include <glm/mat4x4.hpp>

namespace gl
{
class Model
{
public:
    Model();
    Model(const Model &) = delete;
    bool load(const std::wstring &modelFile);
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
    std::vector<Mesh> meshes;
    glm::mat4 translate, rotate, scale;
};

}  // namespace gl
