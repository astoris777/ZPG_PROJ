#include "Window.h"
#include <iostream>

void Window::errorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << std::endl;
}

void Window::mouseButtonCallback(GLFWwindow* winPtr, int button, int action, int mods)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(winPtr));
    if (window && window->inputManager) {
        window->inputManager->handleMouseButton(button, action);
    }
}

void Window::cursorPositionCallback(GLFWwindow* winPtr, double xpos, double ypos)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(winPtr));
    if (window && window->inputManager) {
        window->inputManager->updateMousePosition(xpos, ypos);
    }
}

void Window::framebufferSizeCallback(GLFWwindow* winPtr, int width, int height)
{
    glViewport(0, 0, width, height);
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(winPtr));
    if (window) {
        window->currentWidth = width;
        window->currentHeight = height;
        if (window->camera) {
            window->camera->setResolution(width, height);
        }
    }
}

Window::Window(int width, int height, const char* title)
    : currentWidth(width), currentHeight(height)
{
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    camera = new Camera();
    camera->setResolution(width, height);
    
    inputManager = new InputManager(window, camera);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
}

Window::~Window()
{
    delete inputManager;
    delete camera;
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Window::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

bool Window::isKeyPressed(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}