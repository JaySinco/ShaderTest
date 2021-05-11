#pragma once
#include "shader.h"
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
        Right,
        Up,
        Down,
    };
    Camera(float aspect, glm::vec3 initPos = glm::vec3(0.0f, 0.0f, 3.0f), float near = 0.1f,
           float far = 100.0f, float fov = 45.0f);
    void reset();
    void adaptToScreen(int width, int height);
    void move(Face face, float distance);
    void move(float dx, float dy, float dz);
    void moveTo(float x, float y, float z);
    void shake(float ddegree);
    void shakeTo(float degree);
    void nod(float ddegree);
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
    const glm::vec3 initPos;
    const float near, far, fov;
    float aspect;
    glm::vec3 pos;
    float yaw, pitch;
};

}  // namespace gl
