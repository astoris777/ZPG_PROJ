#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class Material
{
public:
    glm::vec3 color;

    Material(const glm::vec3& col = glm::vec3(1.0f));
    void setUniforms(Shader* shader) const;
};