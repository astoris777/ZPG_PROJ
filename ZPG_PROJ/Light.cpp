#include "Light.h"
#include "Shader.h"

Light::Light(const glm::vec3& pos, const glm::vec3& col, float intens, const glm::vec3& spec)
    : position(pos), color(col), intensity(intens), specular(spec)
{
}

void Light::setUniforms(Shader* shader, const std::string& prefix) const {
    shader->setUniform("lightPosition", position);
    shader->setUniform("lightColor", color);
    shader->setUniform("lightIntensity", intensity);
    shader->setUniform("lightSpecular", specular); 
}

void Light::setUniformsArray(Shader* shader, int index) const {
    std::string prefix = "lights[" + std::to_string(index) + "]";

    shader->setUniform(prefix + ".position", glm::vec4(position, 1.0f));
    shader->setUniform(prefix + ".diffuse", glm::vec4(color, 1.0f));
    shader->setUniform(prefix + ".specular", glm::vec4(specular, 1.0f));
    shader->setUniform(prefix + ".intensity", intensity);
}