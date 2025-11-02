#include "Light.h"
#include "Shader.h"

// Point light constructor
Light::Light(LightType type, const glm::vec3& position, const glm::vec3& color,
    float intensity, float constant, float linear, float quadratic)
    : type(type), position(position), color(color), intensity(intensity),
    constantFactor(constant), linearFactor(linear), quadraticFactor(quadratic),
    direction(0.0f), cutoff(0.0f), outerCutoff(0.0f)
{
    if (type != LightType::POINT) {
        throw std::invalid_argument("Invalid type for POINT light constructor.");
    }
}

// Spot light constructor
Light::Light(LightType type, const glm::vec3& position, const glm::vec3& direction,
    const glm::vec3& color, float intensity, float constant, float linear,
    float quadratic, float cutoff, float outerCutoff)
    : type(type), position(position), direction(glm::normalize(direction)),
    color(color), intensity(intensity), constantFactor(constant),
    linearFactor(linear), quadraticFactor(quadratic), cutoff(cutoff),
    outerCutoff(outerCutoff)
{
    if (type != LightType::SPOT) {
        throw std::invalid_argument("Invalid type for SPOT light constructor.");
    }
}

// Private constructor for directional and ambient
Light::Light(const glm::vec3& directionOrDummy, float intensity, LightType lightType, const glm::vec3& color)
    : type(lightType), color(color), intensity(intensity),
    position(0.0f), constantFactor(1.0f), linearFactor(0.0f), quadraticFactor(0.0f),
    cutoff(0.0f), outerCutoff(0.0f)
{
    // ??????????? direction ?????? ??? directional light
    if (lightType == LightType::DIRECTIONAL) {
        direction = glm::normalize(directionOrDummy);
    } else {
        direction = glm::vec3(0.0f); // Ambient ?? ????? direction
    }
}

// Static factory methods
Light* Light::createPoint(const glm::vec3& position, const glm::vec3& color,
    float intensity, float constant, float linear, float quadratic)
{
    return new Light(LightType::POINT, position, color, intensity, constant, linear, quadratic);
}

Light* Light::createSpot(const glm::vec3& position, const glm::vec3& direction,
    const glm::vec3& color, float intensity, float cutoff, float outerCutoff)
{
    return new Light(LightType::SPOT, position, direction, color, intensity, 1.0f, 0.09f, 0.032f, cutoff, outerCutoff);
}

Light* Light::createDirectional(const glm::vec3& direction, const glm::vec3& color, float intensity)
{
    return new Light(direction, intensity, LightType::DIRECTIONAL, color);
}

Light* Light::createAmbient(const glm::vec3& color, float intensity)
{
    return new Light(glm::vec3(0.0f), intensity, LightType::AMBIENT, color);
}

// Setters
void Light::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Light::setDirection(const glm::vec3& newDirection) {
    direction = glm::normalize(newDirection);
}

void Light::setColor(const glm::vec3& newColor) {
    color = newColor;
}

void Light::setIntensity(float newIntensity) {
    intensity = newIntensity;
}

void Light::setUniformsArray(Shader* shader, int index) const {
    if (type == LightType::AMBIENT) {
        shader->setUniform("ambientLight", color * intensity);
        return;
    }

    std::string prefix = "lights[" + std::to_string(index) + "]";

    shader->setUniform(prefix + ".position", glm::vec4(position, 1.0f));
    shader->setUniform(prefix + ".diffuse", glm::vec4(color, 1.0f));
    shader->setUniform(prefix + ".specular", glm::vec4(color, 1.0f)); // ????????? specular
    shader->setUniform(prefix + ".intensity", intensity);

    // Set light type and specific parameters
    if (type == LightType::SPOT) {
        shader->setUniform(prefix + ".type", 1);
        shader->setUniform(prefix + ".direction", glm::vec4(direction, 0.0f));
        shader->setUniform(prefix + ".cutOff", cutoff);
        shader->setUniform(prefix + ".outerCutOff", outerCutoff);
        shader->setUniform(prefix + ".constant", constantFactor);
        shader->setUniform(prefix + ".linear", linearFactor);
        shader->setUniform(prefix + ".quadratic", quadraticFactor);
    }
    else if (type == LightType::DIRECTIONAL) {
        shader->setUniform(prefix + ".type", 2);
        shader->setUniform(prefix + ".direction", glm::vec4(direction, 0.0f));
        // Directional light ?? ????? attenuation ?????????
    }
    else { // Point light
        shader->setUniform(prefix + ".type", 0);
        shader->setUniform(prefix + ".constant", constantFactor);
        shader->setUniform(prefix + ".linear", linearFactor);
        shader->setUniform(prefix + ".quadratic", quadraticFactor);
    }
}