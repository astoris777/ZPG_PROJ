#pragma once
#include <GLFW/glfw3.h>

class Camera;

class InputManager
{
public:
    InputManager(GLFWwindow* window, Camera* camera);
    void updateMousePosition(double xpos, double ypos);
    void handleMouseButton(int button, int action);

private:
    GLFWwindow* window;
    Camera* camera;
    bool rightMousePressed;
    double lastX, lastY;
};