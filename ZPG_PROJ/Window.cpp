#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const char* title)
{
    camera = new Camera();

    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
        exit(EXIT_FAILURE);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);

    glEnable(GL_DEPTH_TEST);

    inputManager = new InputManager(window, camera);

    glfwSetWindowUserPointer(window, this);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
}

Window::~Window()
{
    delete inputManager;
    delete camera;

    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update()
{
    glfwPollEvents();
    glfwSwapBuffers(window);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

bool Window::isKeyPressed(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void Window::errorCallback(int error, const char* description)
{
    std::cerr << description << std::endl;
}

void Window::mouseButtonCallback(GLFWwindow* winPtr, int button, int action, int mods)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(winPtr));
    win->inputManager->handleMouseButton(button, action);
}

void Window::cursorPositionCallback(GLFWwindow* winPtr, double xpos, double ypos)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(winPtr));
    win->inputManager->updateMousePosition(xpos, ypos);
}