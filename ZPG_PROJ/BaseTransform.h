#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct BaseTransform
{
    virtual ~BaseTransform() = default;
    virtual void apply(glm::mat4& M) const = 0;
    virtual void update(float deltaTime) {}
};

