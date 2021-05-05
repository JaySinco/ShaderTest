#pragma once

namespace gl
{
class Mesh
{
public:
    ~Mesh();
    bool load(size_t vtLen, float *vtData, size_t idLen, unsigned *idData);
    bool draw() const;

private:
    unsigned vao, vbo, ebo;
    unsigned indices;
};

}  // namespace gl
