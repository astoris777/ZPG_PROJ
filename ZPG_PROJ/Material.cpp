#include "Material.h"

Material::Material(const glm::vec3& amb, const glm::vec3& diff,
    const glm::vec3& spec, float shin, Texture* tex)
    : ambient(amb), diffuse(diff), specular(spec), shininess(shin), texture(tex) {
}

void Material::setUniforms(Shader* shader) const {
    shader->setUniform("material.ambient", ambient);
    shader->setUniform("material.diffuse", diffuse);
    shader->setUniform("material.specular", specular);
    shader->setUniform("material.shininess", shininess);

    if (texture) {
        texture->bind(0);
        shader->setUniform("material.hasTexture", 1);
        shader->setUniform("material.diffuseTexture", 0);
    }
    else {
        shader->setUniform("material.hasTexture", 0);
    }
}