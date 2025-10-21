#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class Light
{
public:
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 specular;
    float intensity;


    Light(const glm::vec3& pos, const glm::vec3& col = glm::vec3(1.0f), float intens = 1.0f, const glm::vec3& spec = glm::vec3(1.0f));
    void setUniforms(Shader* shader, const std::string& prefix) const;
    void setUniformsArray(Shader* shader, int index) const;
};