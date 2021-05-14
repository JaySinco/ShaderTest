#pragma once
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include <vector>
#include <string>
#include <memory>
#include <glm/mat4x4.hpp>

namespace gl
{
class Model
{
public:
    bool load(const std::wstring &modelFile);
    void reset();
    void move(float dx, float dy, float dz);
    void spin(float ddegree, float axis_x, float axis_y, float axis_z);
    void zoom(float dx, float dy, float dz);
    void draw(Shader &shader, Camera &camera) const;
    glm::mat4 getModelMatrix() const;

private:
    std::vector<std::shared_ptr<Mesh>> meshes;
    glm::mat4 translate, rotate, scale, init;
};

}  // namespace gl
