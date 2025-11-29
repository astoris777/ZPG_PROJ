#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

class Camera;   
class SceneManager;

class InputManager
{
public:
    InputManager(GLFWwindow* window, Camera* camera);
    void updateMousePosition(double xpos, double ypos);
    void handleMouseButton(int button, int action);
    void setSceneManager(SceneManager* sm);
    void checkDeleteKey();

    std::vector<glm::vec3>& getControlPoints(); 
    
    private:
    GLFWwindow* window;
    Camera* camera;
    SceneManager* sceneManager;
    bool rightMousePressed;
    double lastX, lastY;
    bool deleteKeyPressed;
    
    void handleLeftClick(double xpos, double ypos);
    
    std::vector<glm::vec3> controlPoints;
};