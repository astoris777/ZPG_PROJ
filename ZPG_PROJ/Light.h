#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include <stdexcept>

enum class LightType {
    POINT,
    SPOT,
    DIRECTIONAL,
    AMBIENT
};

class Light
{
public:
    // Point light constructor
    Light(LightType type, const glm::vec3& position, const glm::vec3& color = glm::vec3(1.0f),
        float intensity = 1.0f, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);

    // Spot light constructor
    Light(LightType type, const glm::vec3& position, const glm::vec3& direction,
        const glm::vec3& color = glm::vec3(1.0f), float intensity = 1.0f,
        float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f,
        float cutoff = 0.99f, float outerCutoff = 0.85f);

    // Static factory methods
    static Light* createPoint(const glm::vec3& position, const glm::vec3& color = glm::vec3(1.0f),
        float intensity = 1.0f, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);

    static Light* createSpot(const glm::vec3& position, const glm::vec3& direction,
        const glm::vec3& color = glm::vec3(1.0f), float intensity = 1.0f,
        float cutoff = 0.99f, float outerCutoff = 0.85f);

    static Light* createDirectional(const glm::vec3& direction, const glm::vec3& color = glm::vec3(1.0f),
        float intensity = 1.0f);

    static Light* createAmbient(const glm::vec3& color = glm::vec3(0.1f), float intensity = 1.0f);

    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getDirection() const { return direction; }
    glm::vec3 getColor() const { return color; }
    float getIntensity() const { return intensity; }
    LightType getType() const { return type; }

    // Setters
    void setPosition(const glm::vec3& newPosition);
    void setDirection(const glm::vec3& newDirection);
    void setColor(const glm::vec3& newColor);
    void setIntensity(float newIntensity);

    // ???????? ????? ??? ????????? uniforms
    void setUniformsArray(Shader* shader, int index) const;

private:
    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;

    // Attenuation ??? point/spot light
    float constantFactor;
    float linearFactor;
    float quadraticFactor;

    // Spot light ?????????
    float cutoff;
    float outerCutoff;

    // Private constructor ??? directional ? ambient
    Light(const glm::vec3& direction, float intensity, LightType lightType, const glm::vec3& color);
};