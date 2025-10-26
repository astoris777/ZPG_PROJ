#pragma once
#include "BaseTransform.h"

class TranslateTransform : public BaseTransform
{
    glm::vec3 offset;

public:
    explicit TranslateTransform(const glm::vec3& o) : offset(o) {}
    void apply(glm::mat4& M) const override
    {
        M = glm::translate(M, offset);
    }
};

