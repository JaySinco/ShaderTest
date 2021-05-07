#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace gl
{
class Camera
{
public:
    enum Face
    {
        Forward,
        Backward,
        Left,
        Right
    };
    Camera(float aspect, float near = 0.1f, float far = 100.0f, float fov = 45.0f);
    void move(Face face, float distance);
    void moveTo(float x, float y, float z);
    void shake(float degree);
    void shakeTo(float degree);
    void nod(float degree);
    void nodTo(float degree);
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewProjectionMatrix() const;

private:
    struct Axis
    {
        glm::vec3 front, right, up;
    };
    Axis getAxis() const;
    glm::vec3 pos;
    glm::mat4 projection;
    float yaw = -90.0f;
    float pitch = 0.0f;
};

}  // namespace gl
