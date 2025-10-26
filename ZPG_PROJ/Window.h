#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "InputManager.h"

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    void clear(float r, float g, float b, float a);
    void update();
    bool shouldClose();
    bool isKeyPressed(int key);

    int getWidth() const { return currentWidth; }
    int getHeight() const { return currentHeight; }
    float getAspectRatio() const { return static_cast<float>(currentWidth) / static_cast<float>(currentHeight); }

    GLFWwindow* window;
    Camera* camera;

private:
    InputManager* inputManager;
    int currentWidth, currentHeight;

    static void errorCallback(int error, const char* description);
    static void mouseButtonCallback(GLFWwindow* winPtr, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* winPtr, double xpos, double ypos);
    static void framebufferSizeCallback(GLFWwindow* winPtr, int width, int height);
};