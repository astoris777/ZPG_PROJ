#pragma once
#include "BaseTransform.h"

class CustomTransform : public BaseTransform
{
    glm::mat4 customMatrix;

public:
    explicit CustomTransform(const glm::mat4& matrix = glm::mat4(1.0f))
        : customMatrix(matrix) {
    }

    explicit CustomTransform(float wValue)
    {
        customMatrix = glm::mat4(1.0f);  
        customMatrix[3][3] = wValue;     
    }

    void apply(glm::mat4& M) const override
    {
        M = M * customMatrix;
    }

    void setMatrix(const glm::mat4& matrix)
    {
        customMatrix = matrix;
    }

    const glm::mat4& getMatrix() const
    {
        return customMatrix;
    }
};