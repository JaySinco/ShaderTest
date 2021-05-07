#include "camera.h"
#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>

namespace gl
{
Camera::Camera(float aspect, float near, float far, float fov)
{
    this->projection = glm::perspective(glm::radians(fov), aspect, near, far);
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
            this->pos += axis.right * distance;
            break;
        case Right:
            this->pos += axis.right * distance;
            break;
    }
}

void Camera::moveTo(float x, float y, float z) { this->pos = glm::vec3(x, y, z); }

void Camera::shake(float degree) { this->yaw += degree; }

void Camera::shakeTo(float degree) { this->shake(degree - this->yaw); }

void Camera::nod(float degree)
{
    this->pitch = std::max(std::min(this->pitch + degree, 89.0f), -89.0f);
}

void Camera::nodTo(float degree) { this->nod(degree - pitch); }

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

glm::mat4 Camera::getProjectionMatrix() const { return this->projection; }

glm::mat4 Camera::getViewProjectionMatrix() const
{
    return this->getProjectionMatrix() * this->getViewMatrix();
}

}  // namespace gl
