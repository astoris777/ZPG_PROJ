#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "Camera.h"

Camera::Camera()
    : position(0.0f, 1.0f, 5.0f),
    front(0.0f, 0.0f, -1.0f),
    up(0.0f, 1.0f, 0.0f),
    yaw(-90.0f),
    pitch(0.0f),
    sensitivity(0.1f)
{
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::attach(ShaderObserver* observer) {
    observers.push_back(observer);
}

void Camera::detach(ShaderObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Camera::notify(const glm::mat4& projection) {
    glm::mat4 view = getViewMatrix();
    for (auto obs : observers)
        obs->update(view, projection);
}

void Camera::moveForward(float delta) {
    position += front * delta;
}

void Camera::moveBackward(float delta) {
    position -= front * delta;
}

void Camera::moveRight(float delta) {
    position += glm::normalize(glm::cross(front, up)) * delta;
}

void Camera::moveLeft(float delta) {
    position -= glm::normalize(glm::cross(front, up)) * delta;
}

glm::vec3 Camera::getPosition()
{
    return position;
}