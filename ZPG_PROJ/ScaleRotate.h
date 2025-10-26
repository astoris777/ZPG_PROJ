#pragma once
#include "BaseTransform.h"

class ScaleTransform : public BaseTransform
{
    glm::vec3 factors;
    glm::vec3 speed;

public:
    ScaleTransform(const glm::vec3& f, const glm::vec3& spd = glm::vec3(0.0f))
        : factors(f), speed(spd) {
    }

    void apply(glm::mat4& M) const override
    {
        M = glm::scale(M, factors);
    }

    void update(float deltaTime) override
    {
        factors += speed * deltaTime;

        if (factors.x < 0.1f)
            factors.x = 0.1f;
        if (factors.y < 0.1f)
            factors.y = 0.1f;
        if (factors.z < 0.1f)
            factors.z = 0.1f;
    }
};