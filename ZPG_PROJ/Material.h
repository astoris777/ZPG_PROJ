#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class Material
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Material(const glm::vec3& amb = glm::vec3(1.0f), const glm::vec3& diff = glm::vec3(1.0f),
        const glm::vec3& spec = glm::vec3(1.0f), float shin = 32.0f);
         
        void setUniforms(Shader* shader) const;
};