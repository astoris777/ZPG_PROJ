#pragma once
#include <vector>
#include "BaseTransform.h"

class Transform
{
public:
    std::vector<BaseTransform*> transforms;

    void add(BaseTransform* t) { transforms.push_back(t); }

    void update(float deltaTime)
    {
        for (auto t : transforms)
            t->update(deltaTime);
    }

    glm::mat4 getModelMatrix() const
    {
        glm::mat4 M(1.0f);
        for (auto t : transforms)
            t->apply(M);
        return M;
    }

    ~Transform()
    {
        for (auto t : transforms)
            delete t;
    }
};