#include "Material.h"

Material::Material(const glm::vec3& amb, const glm::vec3& diff, 
                   const glm::vec3& spec, float shin)
    : ambient(amb), diffuse(diff), specular(spec), shininess(shin) {}

void Material::setUniforms(Shader* shader) const {
    shader->setUniform("material.ambient", ambient);
    shader->setUniform("material.diffuse", diffuse);
    shader->setUniform("material.specular", specular);
    shader->setUniform("material.shininess", shininess);
}