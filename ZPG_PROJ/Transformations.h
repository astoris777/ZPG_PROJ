#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct BaseTransform
{
    virtual ~BaseTransform() = default;
    virtual void apply(glm::mat4& M) const = 0;
    virtual void update(float deltaTime) {}
};

class TranslateTransform : public BaseTransform
{
    glm::vec3 offset;

public:
    explicit TranslateTransform(const glm::vec3& o) : offset(o) {}
    void apply(glm::mat4& M) const override
    {
        M = glm::translate(M, offset);
    }
};

class RotateTransform : public BaseTransform
{
    float angle;
    glm::vec3 axis;
    float speed;

public:
    RotateTransform(float a, const glm::vec3& ax, float spd = 0.0f) : angle(a), axis(ax), speed(spd) {}
    void apply(glm::mat4& M) const override
    {
        M = glm::rotate(M, glm::radians(angle), axis);
    }
    void update(float deltaTime) override
    {
        angle += speed * deltaTime;
        if (angle >= 360.0f)
            angle -= 360.0f;
    }
};

class ScaleTransform : public BaseTransform
{
    glm::vec3 factors;
    glm::vec3 speed;

public:
    ScaleTransform(const glm::vec3& f, const glm::vec3& spd = glm::vec3(0.0f))
        : factors(f), speed(spd) {
    }

    void apply(glm::mat4& M) const override
    {
        M = glm::scale(M, factors);
    }

    void update(float deltaTime) override
    {
        factors += speed * deltaTime;

        if (factors.x < 0.1f)
            factors.x = 0.1f;
        if (factors.y < 0.1f)
            factors.y = 0.1f;
        if (factors.z < 0.1f)
            factors.z = 0.1f;
    }
};

class Transform
{
public:
    std::vector<BaseTransform*> transforms;

    void add(BaseTransform* t) { transforms.push_back(t); }

    void update(float deltaTime)
    {
        for (auto t : transforms)
            t->update(deltaTime);
    }

    glm::mat4 getModelMatrix() const
    {
        glm::mat4 M(1.0f);
        for (auto t : transforms)
            t->apply(M);
        return M;
    }

    ~Transform()
    {
        for (auto t : transforms)
            delete t;
    }
};