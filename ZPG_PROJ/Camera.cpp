#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

Camera::Camera()
    : position(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f),
      yaw(-90.0f), pitch(0.0f), sensitivity(0.1f), resolutionX(800), resolutionY(600)
{
    moveSpeed = 0.05f;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::attach(ShaderObserver *observer)
{
    observers.push_back(observer);
}

void Camera::detach(ShaderObserver *observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Camera::notify(const glm::mat4 &projection)
{
    glm::mat4 view = getViewMatrix();
    for (auto observer : observers)
    {
        observer->update(view, projection);
    }
}

void Camera::moveForward(float delta)
{
    position += front * moveSpeed * delta;
}

void Camera::moveBackward(float delta)
{
    position -= front * moveSpeed * delta;
}

void Camera::moveRight(float delta)
{
    position += glm::normalize(glm::cross(front, up)) * moveSpeed * delta;
}

void Camera::moveLeft(float delta)
{
    position -= glm::normalize(glm::cross(front, up)) * moveSpeed * delta;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

glm::vec3 Camera::getPosition()
{
    return position;
}

void Camera::setResolution(int width, int height)
{
    resolutionX = width;
    resolutionY = height;
}

glm::ivec2 Camera::getResolution() const
{
    return glm::ivec2(resolutionX, resolutionY);
}

void Camera::setPosition(const glm::vec3 &pos)
{
    position = pos;
}

void Camera::lookAt(const glm::vec3 &target)
{
    glm::vec3 direction = glm::normalize(target - position);
    yaw = glm::degrees(atan2(direction.z, direction.x));
    pitch = glm::degrees(asin(direction.y));

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
}

void Camera::setSensitivity(float sens)
{
    sensitivity = sens;
}

void Camera::setMoveSpeed(float speed)
{
    moveSpeed = speed;
}

float Camera::getMoveSpeed() const
{
    return moveSpeed;
}