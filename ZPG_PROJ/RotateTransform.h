#pragma once
#include "BaseTransform.h"

class RotateTransform : public BaseTransform
{
    float angle;
    glm::vec3 axis;
    float speed;

public:
    RotateTransform(float a, const glm::vec3& ax, float spd = 0.0f) : angle(a), axis(ax), speed(spd) {}
    void apply(glm::mat4& M) const override
    {
        M = glm::rotate(M, glm::radians(angle), axis);
    }
    void update(float deltaTime) override
    {
        angle += speed * deltaTime;
        if (angle >= 360.0f)
            angle -= 360.0f;
    }
};