#include "InputManager.h"
#include "Camera.h"
#include "SceneManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <vector>


InputManager::InputManager(GLFWwindow* window, Camera* camera)
    : window(window), camera(camera), sceneManager(nullptr), rightMousePressed(false), lastX(0.0), lastY(0.0), deleteKeyPressed(false)
{
}

void InputManager::setSceneManager(SceneManager* sm)
{
    sceneManager = sm;
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
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        handleLeftClick(xpos, ypos);
    }
}

void InputManager::checkDeleteKey()
{
    if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS && !deleteKeyPressed) {
        deleteKeyPressed = true;
        if (sceneManager) {
            sceneManager->deleteSelectedObject();
            printf("Object deleted!\n");
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_RELEASE) {
        deleteKeyPressed = false;
    }
}

void InputManager::handleLeftClick(double xpos, double ypos)
{
    if (!sceneManager) return;

    GLbyte color[4];
    GLfloat depth;
    GLuint index;

    GLint x = static_cast<GLint>(xpos);
    GLint y = static_cast<GLint>(ypos);

    glm::ivec2 resolution = camera->getResolution();
    int newy = resolution.y - y;

    glReadPixels(x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
           x, y, color[0], color[1], color[2], color[3], depth, index);

    if (index > 0) {
        sceneManager->setSelectedObject(index);
        sceneManager->deleteSelectedObject();
    }

    glm::vec3 screenX = glm::vec3(x, newy, depth);
    glm::mat4 view = camera->getViewMatrix();
    float aspectRatio = static_cast<float>(resolution.x) / static_cast<float>(resolution.y);
    glm::mat4 projection = glm::perspective(glm::radians(sceneManager->getCurrentFOV()), aspectRatio, 0.1f, 100.0f);
    glm::vec4 viewPort = glm::vec4(0, 0, resolution.x, resolution.y);
    glm::vec3 pos = glm::unProject(screenX, view, projection, viewPort);

    printf("unProject [%f,%f,%f]\n", pos.x, pos.y, pos.z);

    controlPoints.push_back(pos);
}

std::vector<glm::vec3>& InputManager::getControlPoints()
{
    return controlPoints;
}