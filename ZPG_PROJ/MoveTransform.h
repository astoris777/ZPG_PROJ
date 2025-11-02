#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

enum class MoveType {
    CIRCULAR,
    LINEAR,
    SINUSOIDAL,
    FIGURE_EIGHT,
    ELLIPTICAL
};

class MoveTransform : public BaseTransform
{
private:
    MoveType moveType;
    glm::vec3 center;
    glm::vec3 startPos;
    glm::vec3 endPos;
    glm::vec3 amplitude;
    float radius;
    float radiusX;
    float radiusZ;
    float speed;
    float frequency;
    float elapsedTime = 0.0f;

public:
    // ???????????? ??? ?????? ????? ????????
    static MoveTransform* createCircular(const glm::vec3& center = glm::vec3(0.0f), float radius = 3.0f, float speed = 2.0f)
    {
        MoveTransform* transform = new MoveTransform();
        transform->moveType = MoveType::CIRCULAR;
        transform->center = center;
        transform->radius = radius;
        transform->speed = speed;
        return transform;
    }

    static MoveTransform* createLinear(const glm::vec3& start, const glm::vec3& end, float speed = 1.0f)
    {
        MoveTransform* transform = new MoveTransform();
        transform->moveType = MoveType::LINEAR;
        transform->startPos = start;
        transform->endPos = end;
        transform->speed = speed;
        return transform;
    }

    static MoveTransform* createSinusoidal(const glm::vec3& basePos, const glm::vec3& amplitude, float frequency = 1.0f)
    {
        MoveTransform* transform = new MoveTransform();
        transform->moveType = MoveType::SINUSOIDAL;
        transform->center = basePos;
        transform->amplitude = amplitude;
        transform->frequency = frequency;
        return transform;
    }

    void apply(glm::mat4& M) const override
    {
        switch (moveType)
        {
        case MoveType::CIRCULAR:
        {
            float angle = speed * elapsedTime;
            float x = center.x + radius * cos(angle);
            float z = center.z + radius * sin(angle);
            M = glm::translate(M, glm::vec3(x, center.y, z));
            break;
        }
        case MoveType::LINEAR:
        {
            float t = fmod(speed * elapsedTime, 2.0f);
            if (t > 1.0f) t = 2.0f - t;
            glm::vec3 currentPos = glm::mix(startPos, endPos, t);
            M = glm::translate(M, currentPos);
            break;
        }
        case MoveType::SINUSOIDAL:
        {
            glm::vec3 offset;
            offset.x = amplitude.x * sin(frequency * elapsedTime);
            offset.y = amplitude.y * sin(frequency * elapsedTime + glm::pi<float>() / 2.0f);
            offset.z = amplitude.z * sin(frequency * elapsedTime + glm::pi<float>());
            M = glm::translate(M, center + offset);
            break;
        }
        }
    }

    void update(float deltaTime) override
    {
        elapsedTime += deltaTime;
    }

    // ???????????? ??????????? ?????? ??? ??????? ?????????????
    static void CircularMove(glm::mat4& M, const glm::vec3& center, float radius, float speed, float time)
    {
        float angle = speed * time;
        float x = center.x + radius * cos(angle);
        float z = center.z + radius * sin(angle);
        M = glm::translate(M, glm::vec3(x, center.y, z));
    }

private:
    MoveTransform() = default; // ????????? ??????????? ??? ????????????? ?????? ????? ???????
};