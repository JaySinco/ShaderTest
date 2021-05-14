#include "camera.h"
#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>

namespace gl
{
Camera::Camera(float aspect, glm::vec3 initPos, float near, float far, float fov)
    : aspect(aspect), initPos(initPos), near(near), far(far), fov(fov)
{
    this->reset();
}

void Camera::reset()
{
    this->pos = this->initPos;
    this->yaw = -90.0f;
    this->pitch = 0.0f;
}

void Camera::adaptToScreen(int width, int height)
{
    this->aspect = static_cast<float>(width) / height;
}

void Camera::move(Face face, float distance)
{
    Axis axis = this->getAxis();
    switch (face) {
        case Forward:
            this->pos += axis.front * distance;
            break;
        case Backward:
            this->pos -= axis.front * distance;
            break;
        case Left:
            this->pos -= axis.right * distance;
            break;
        case Right:
            this->pos += axis.right * distance;
            break;
        case Up:
            this->pos += axis.up * distance;
            break;
        case Down:
            this->pos -= axis.up * distance;
            break;
    }
}

void Camera::move(float dx, float dy, float dz) { this->pos += glm::vec3(dx, dy, dz); }

void Camera::shake(float ddegree) { this->yaw += ddegree; }

void Camera::nod(float ddegree)
{
    this->pitch = std::max(std::min(this->pitch + ddegree, 89.0f), -89.0f);
}

Camera::Axis Camera::getAxis() const
{
    Axis axis;
    float x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    float y = sin(glm::radians(this->pitch));
    float z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    axis.front = glm::normalize(glm::vec3(x, y, z));
    axis.right = glm::normalize(glm::cross(axis.front, glm::vec3(0.0f, 1.0f, 0.0f)));
    axis.up = glm::normalize(glm::cross(axis.right, axis.front));
    return axis;
}

glm::mat4 Camera::getViewMatrix() const
{
    Axis axis = this->getAxis();
    return glm::lookAt(this->pos, this->pos + axis.front, axis.up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(this->fov), this->aspect, this->near, this->far);
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
    return this->getProjectionMatrix() * this->getViewMatrix();
}

}  // namespace gl
