#include "InputManager.h"
#include "Camera.h"

InputManager::InputManager(GLFWwindow* window, Camera* camera)
    : window(window), camera(camera), rightMousePressed(false), lastX(0.0), lastY(0.0)
{
}

void InputManager::updateMousePosition(double xpos, double ypos)
{
    if (!rightMousePressed) return;

    double deltaX = xpos - lastX;
    double deltaY = ypos - lastY;

    camera->processMouseMovement(deltaX, deltaY);

    lastX = xpos;
    lastY = ypos;
}

void InputManager::handleMouseButton(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            rightMousePressed = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        }
        else if (action == GLFW_RELEASE)
        {
            rightMousePressed = false;
        }
    }
}