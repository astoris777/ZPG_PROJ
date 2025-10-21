#include "Application.h"
#include <GLFW/glfw3.h>
#include <iostream>

Application::Application()
    : window(nullptr), sceneManager(nullptr), resourceManager(nullptr)
{
}

Application::~Application()
{
    delete window;
    delete sceneManager;
    delete resourceManager;
}

void Application::initialization(int width, int height)
{
    window = new Window(width, height, "Opengl Application");
    resourceManager = new ResourceManager();
    sceneManager = new SceneManager(resourceManager, window->camera);

    resourceManager->createShaders();
    resourceManager->createModels();
    sceneManager->createScenes();
}

void Application::processInput(float deltaTime)
{
    float cameraSpeed = 2.5f * deltaTime;

    if (window->isKeyPressed(GLFW_KEY_W))
        window->camera->moveForward(cameraSpeed);
    if (window->isKeyPressed(GLFW_KEY_S))
        window->camera->moveBackward(cameraSpeed);
    if (window->isKeyPressed(GLFW_KEY_A))
        window->camera->moveLeft(cameraSpeed);
    if (window->isKeyPressed(GLFW_KEY_D))
        window->camera->moveRight(cameraSpeed);
    if (window->isKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window->window, true);

    if (window->isKeyPressed(GLFW_KEY_1))
    {
        sceneManager->switchScene(0);
    }
    if (window->isKeyPressed(GLFW_KEY_2))
    {
        sceneManager->switchScene(1);
    }
    if (window->isKeyPressed(GLFW_KEY_3))
    {
        sceneManager->switchScene(2);
    }
    if (window->isKeyPressed(GLFW_KEY_4))
    {
        sceneManager->switchScene(3);
    }

    if (window->isKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window->window, true);
}

void Application::run()
{
    float lastFrame = glfwGetTime();

    while (!window->shouldClose())
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(deltaTime);
        sceneManager->update(deltaTime);

        window->clear(0.0f, 0.0f, 0.0f, 1.0f);
        sceneManager->render();
        window->update();
    }
}
