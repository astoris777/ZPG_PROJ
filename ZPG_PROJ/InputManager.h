#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera;
class SceneManager;

class InputManager
{
public:
    InputManager(GLFWwindow* window, Camera* camera);
    void updateMousePosition(double xpos, double ypos);
    void handleMouseButton(int button, int action);
    void setSceneManager(SceneManager* sm);

private:
    GLFWwindow* window;
    Camera* camera;
    SceneManager* sceneManager;
    bool rightMousePressed;
    double lastX, lastY;
    
    void handleLeftClick(double xpos, double ypos);
};