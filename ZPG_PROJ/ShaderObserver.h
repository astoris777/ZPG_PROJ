#pragma once
#include <glm/glm.hpp>

class ShaderObserver {
public:
    virtual void update(const glm::mat4& view, const glm::mat4& projection) = 0;
    virtual ~ShaderObserver() = default;
};