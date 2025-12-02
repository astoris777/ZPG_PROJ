#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "ShaderObserver.h"

class Camera
{
private:
    std::vector<ShaderObserver *> observers;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    float sensitivity;
    int resolutionX;
    int resolutionY;
    float moveSpeed;

public:
    Camera();
    glm::mat4 getViewMatrix() const;
    void attach(ShaderObserver *observer);
    void detach(ShaderObserver *observer);
    void notify(const glm::mat4 &projection);
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveRight(float delta);
    void moveLeft(float delta);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    glm::vec3 getPosition();
    void setResolution(int width, int height);
    glm::ivec2 getResolution() const;

    void setPosition(const glm::vec3 &pos);
    void lookAt(const glm::vec3 &target);
    void setSensitivity(float sens);
    void setMoveSpeed(float speed);
    float getMoveSpeed() const;
};
