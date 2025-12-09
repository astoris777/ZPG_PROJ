#pragma once
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <vector>

enum class MoveType
{
    CIRCULAR,
    LINEAR,
    SINUSOIDAL,
    POLYLINE,
    BEZIER,
    BEZIER_SPLINE
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
    float speed;
    float frequency;
    float elapsedTime = 0.0f;
    std::vector<glm::vec3> controlPoints;
    float t = 0.0f;
    bool loopBezier = true;
    float delta = 0.01f;
        bool loopPath;

    float totalPathLength;

    std::vector<float> segmentLengths;
public:
    static MoveTransform *createCircular(const glm::vec3 &center = glm::vec3(0.0f), float radius = 3.0f, float speed = 2.0f)
    {
        MoveTransform *transform = new MoveTransform();
        transform->moveType = MoveType::CIRCULAR;
        transform->center = center;
        transform->radius = radius;
        transform->speed = speed;
        return transform;
    }

    static MoveTransform *createLinear(const glm::vec3 &start, const glm::vec3 &end, float speed = 1.0f)
    {
        MoveTransform *transform = new MoveTransform();
        transform->moveType = MoveType::LINEAR;
        transform->startPos = start;
        transform->endPos = end;
        transform->speed = speed;
        return transform;
    }

    static MoveTransform *createSinusoidal(const glm::vec3 &basePos, const glm::vec3 &amplitude, float frequency = 1.0f)
    {
        MoveTransform *transform = new MoveTransform();
        transform->moveType = MoveType::SINUSOIDAL;
        transform->center = basePos;
        transform->amplitude = amplitude;
        transform->frequency = frequency;
        return transform;
    }

        static MoveTransform* createPolyline(const std::vector<glm::vec3>& points, float speed = 1.0f, bool loop = true)

    {

        MoveTransform* transform = new MoveTransform();

        transform->moveType = MoveType::POLYLINE;

        transform->controlPoints = points;

        transform->speed = speed;

        transform->loopPath = loop;

        transform->totalPathLength = 0.0f;

        transform->segmentLengths.clear();

        for (size_t i = 0; i < points.size() - 1; ++i) {

            float segmentLen = glm::length(points[i + 1] - points[i]);

            transform->segmentLengths.push_back(segmentLen);

            transform->totalPathLength += segmentLen;

        }

        if (loop && points.size() > 1) {

            float segmentLen = glm::length(points[0] - points.back());

            transform->segmentLengths.push_back(segmentLen);

            transform->totalPathLength += segmentLen;

        }

        return transform;

    }


    static MoveTransform *createBezier(const std::vector<glm::vec3> &controlPoints, float speed = 1.0f, bool loop = true)
    {
        MoveTransform *transform = new MoveTransform();
        transform->moveType = MoveType::BEZIER;
        transform->controlPoints = controlPoints;
        transform->speed = speed;
        transform->loopBezier = loop;
        transform->t = 0.0f;
        transform->delta = 0.01f;
        return transform;
    }

    static MoveTransform *createBezierSpline(const std::vector<glm::vec3> &controlPoints, float speed = 1.0f, bool loop = true)
    {
        MoveTransform *transform = new MoveTransform();
        transform->moveType = MoveType::BEZIER_SPLINE;
        transform->controlPoints = controlPoints;
        transform->speed = speed;
        transform->loopBezier = loop;
        transform->t = 0.0f;
        transform->delta = 0.01f;
        return transform;
    }

    void apply(glm::mat4 &M) const override
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
            if (t > 1.0f)
                t = 2.0f - t;
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
                case MoveType::POLYLINE:

        {

            if (controlPoints.size() < 2) {

                if (!controlPoints.empty()) {

                    M = glm::translate(M, controlPoints[0]);

                }

                break;

            }

            float distance = fmod(speed * elapsedTime, totalPathLength);

            if (!loopPath && speed * elapsedTime > totalPathLength) {

                M = glm::translate(M, controlPoints.back());

                break;

            }

            float accumulatedLength = 0.0f;

            size_t segmentIndex = 0;

            for (size_t i = 0; i < segmentLengths.size(); ++i) {

                if (distance < accumulatedLength + segmentLengths[i]) {

                    segmentIndex = i;

                    break;

                }

                accumulatedLength += segmentLengths[i];

            }

            float segmentT = (distance - accumulatedLength) / segmentLengths[segmentIndex];

            glm::vec3 startPoint = controlPoints[segmentIndex];

            glm::vec3 endPoint;

            if (segmentIndex < controlPoints.size() - 1) {

                endPoint = controlPoints[segmentIndex + 1];

            }

            else {

                endPoint = controlPoints[0];

            }

            glm::vec3 currentPos = glm::mix(startPoint, endPoint, segmentT);

            M = glm::translate(M, currentPos);

            break;
        }

        case MoveType::BEZIER:
        {
            if (controlPoints.size() < 4)
                break;

            glm::mat4 A = glm::mat4(
                glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f),
                glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
                glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

            glm::mat4x3 B = glm::mat4x3(
                glm::vec3(controlPoints[0]),
                glm::vec3(controlPoints[1]),
                glm::vec3(controlPoints[2]),
                glm::vec3(controlPoints[3]));

            glm::vec4 p = glm::vec4(
                t * t * t,
                t * t,
                t,
                1.0f);

            glm::vec3 point = p * A * glm::transpose(B);

            M = glm::translate(M, point);

            break;
        }

        case MoveType::BEZIER_SPLINE:
        {
            if (controlPoints.size() < 4)
                break;

            int numSegments = controlPoints.size() / 4;

            float clampedT = glm::clamp(t, 0.0f, 1.0f);

            float globalT = clampedT * numSegments;
            int currentSegment = (int)globalT;

            if (currentSegment >= numSegments)
                currentSegment = numSegments - 1;

            float localT = globalT - currentSegment;

            int baseIndex = currentSegment * 4;
            glm::vec3 p0 = controlPoints[baseIndex];
            glm::vec3 p1 = controlPoints[baseIndex + 1];
            glm::vec3 p2 = controlPoints[baseIndex + 2];
            glm::vec3 p3 = controlPoints[baseIndex + 3];

            glm::mat4 A = glm::mat4(
                glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f),
                glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
                glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
                glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

            glm::mat4x3 B = glm::mat4x3(p0, p1, p2, p3);

            glm::vec4 p = glm::vec4(
                localT * localT * localT,
                localT * localT,
                localT,
                1.0f);

            glm::vec3 point = p * A * glm::transpose(B);

            M = glm::translate(M, point);

            break;
        }

        default:
            break;
        }
    }

    void update(float deltaTime) override
    {
        elapsedTime += deltaTime;

        if (moveType == MoveType::BEZIER || moveType == MoveType::BEZIER_SPLINE)
        {
            if (t > 1.0f || t <= 0.0f)
            {
                delta *= -1.0f;
            }

            t += delta * speed;
        }
    }

private:
    MoveTransform() = default;
};