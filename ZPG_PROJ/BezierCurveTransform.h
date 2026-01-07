#pragma once
#include "BaseTransform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

/**
 * @brief Трансформация движения объекта по кубической кривой Безье.
 * 
 * Использует матричную форму кубической кривой Безье:
 * P(t) = T * A * B
 * где T = [t³, t², t, 1], A - матрица коэффициентов Безье, B - контрольные точки
 */
class BezierCurveTransform : public BaseTransform
{
private:
    // Контрольные точки кривой Безье (нужно 4 точки для кубической кривой)
    std::vector<glm::vec3> controlPoints;
    
    // Текущий параметр t (0.0 - 1.0)
    float t;
    
    // Скорость изменения параметра t
    float speed;
    
    // Направление движения (+1 или -1)
    float direction;
    
    // Зацикливать ли движение (true = туда-обратно)
    bool loop;
    
    // Матрица коэффициентов кубической кривой Безье
    static const glm::mat4& getBezierMatrix()
    {
        static const glm::mat4 A = glm::mat4(
            glm::vec4(-1.0f,  3.0f, -3.0f, 1.0f),
            glm::vec4( 3.0f, -6.0f,  3.0f, 0.0f),
            glm::vec4(-3.0f,  3.0f,  0.0f, 0.0f),
            glm::vec4( 1.0f,  0.0f,  0.0f, 0.0f)
        );
        return A;
    }
    
    // Вычислить позицию на кривой для заданного t
    glm::vec3 calculatePoint(float param) const
    {
        if (controlPoints.size() < 4)
            return glm::vec3(0.0f);
        
        // Матрица контрольных точек
        glm::mat4x3 B = glm::mat4x3(
            controlPoints[0],
            controlPoints[1],
            controlPoints[2],
            controlPoints[3]
        );
        
        // Вектор параметра [t³, t², t, 1]
        glm::vec4 T = glm::vec4(
            param * param * param,
            param * param,
            param,
            1.0f
        );
        
        // P(t) = T * A * B^T
        return T * getBezierMatrix() * glm::transpose(B);
    }
    
    // Приватный конструктор для паттерна фабрики
    BezierCurveTransform() 
        : t(0.0f)
        , speed(1.0f)
        , direction(1.0f)
        , loop(true) 
    {}

public:
    /**
     * @brief Создать трансформацию с 4 контрольными точками
     * @param p0 Начальная точка
     * @param p1 Первая контрольная точка
     * @param p2 Вторая контрольная точка
     * @param p3 Конечная точка
     * @param speed Скорость движения (по умолчанию 1.0)
     * @param loop Зацикливать движение (по умолчанию true)
     */
    static BezierCurveTransform* create(
        const glm::vec3& p0,
        const glm::vec3& p1,
        const glm::vec3& p2,
        const glm::vec3& p3,
        float speed = 1.0f,
        bool loop = true)
    {
        BezierCurveTransform* transform = new BezierCurveTransform();
        transform->controlPoints = { p0, p1, p2, p3 };
        transform->speed = speed;
        transform->loop = loop;
        return transform;
    }
    
    /**
     * @brief Создать трансформацию с вектором контрольных точек
     * @param points Вектор из 4 контрольных точек
     * @param speed Скорость движения
     * @param loop Зацикливать движение
     */
    static BezierCurveTransform* create(
        const std::vector<glm::vec3>& points,
        float speed = 1.0f,
        bool loop = true)
    {
        if (points.size() < 4)
            return nullptr;
        
        BezierCurveTransform* transform = new BezierCurveTransform();
        transform->controlPoints = points;
        transform->speed = speed;
        transform->loop = loop;
        return transform;
    }
    
    /**
     * @brief Применить трансформацию к матрице модели
     */
    void apply(glm::mat4& M) const override
    {
        glm::vec3 position = calculatePoint(t);
        M = glm::translate(M, position);
    }
    
    /**
     * @brief Обновить параметр t на основе deltaTime
     */
    void update(float deltaTime) override
    {
        t += direction * speed * deltaTime;
        
        if (loop)
        {
            // Движение туда-обратно
            if (t >= 1.0f)
            {
                t = 1.0f;
                direction = -1.0f;
            }
            else if (t <= 0.0f)
            {
                t = 0.0f;
                direction = 1.0f;
            }
        }
        else
        {
            // Однократное движение с остановкой в конце
            t = glm::clamp(t, 0.0f, 1.0f);
        }
    }
    
    // Геттеры и сеттеры
    float getT() const { return t; }
    void setT(float newT) { t = glm::clamp(newT, 0.0f, 1.0f); }
    
    float getSpeed() const { return speed; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
    
    bool isLooping() const { return loop; }
    void setLoop(bool newLoop) { loop = newLoop; }
    
    glm::vec3 getCurrentPosition() const { return calculatePoint(t); }
    
    const std::vector<glm::vec3>& getControlPoints() const { return controlPoints; }
    
    void setControlPoints(const std::vector<glm::vec3>& points)
    {
        if (points.size() >= 4)
            controlPoints = points;
    }
};
