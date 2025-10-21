#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "ShaderObserver.h"

class Camera {
private:
    std::vector<ShaderObserver*> observers;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    float sensitivity;

public:
    Camera();
    glm::mat4 getViewMatrix() const;
    void attach(ShaderObserver* observer);
    void detach(ShaderObserver* observer);
    void notify(const glm::mat4& projection);
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveRight(float delta);
    void moveLeft(float delta);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    glm::vec3 getPosition();
};
