#include "Material.h"

Material::Material(const glm::vec3& col)
    : color(col)
{
}

void Material::setUniforms(Shader* shader) const {
    shader->setUniform("objectColor", color);
}