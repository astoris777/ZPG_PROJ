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
    sceneManager = new SceneManager(resourceManager, window->camera, window);

    resourceManager->createShaders();
    resourceManager->createModels();
    resourceManager->attachShadersToCamera(window->camera);
    sceneManager->createScenes();
}

void Application::processInput(float deltaTime)
{
    float cameraSpeed = 2.5f * deltaTime;

    // ?????????? ???????
    if (window->isKeyPressed(GLFW_KEY_W))
        window->camera->moveForward(cameraSpeed);
    if (window->isKeyPressed(GLFW_KEY_S))
        window->camera->moveBackward(cameraSpeed);
    if (window->isKeyPressed(GLFW_KEY_A))
        window->camera->moveLeft(cameraSpeed);
    if (window->isKeyPressed(GLFW_KEY_D))
        window->camera->moveRight(cameraSpeed);

    // ?????????? FOV ? ??????? ?????????????? ??????
    static bool f1Pressed = false, f2Pressed = false, f3Pressed = false;

    if (window->isKeyPressed(GLFW_KEY_F1) && !f1Pressed) {
        sceneManager->setFOV45();
        f1Pressed = true;
    } else if (!window->isKeyPressed(GLFW_KEY_F1)) {
        f1Pressed = false;
    }

    if (window->isKeyPressed(GLFW_KEY_F2) && !f2Pressed) {
        sceneManager->setFOV90();
        f2Pressed = true;
    } else if (!window->isKeyPressed(GLFW_KEY_F2)) {
        f2Pressed = false;
    }

    if (window->isKeyPressed(GLFW_KEY_F3) && !f3Pressed) {
        sceneManager->setFOV130();
        f3Pressed = true;
    } else if (!window->isKeyPressed(GLFW_KEY_F3)) {
        f3Pressed = false;
    }

    // ???????????? ????
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

    // ????? ?? ??????????
    if (window->isKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window->window, true);
}

void Application::run()
{
    float lastFrame = glfwGetTime();

    std::cout << "=== FOV Control ===" << std::endl;
    std::cout << "F1 - FOV 45°" << std::endl;
    std::cout << "F2 - FOV 90°" << std::endl;
    std::cout << "F3 - FOV 130°" << std::endl;
    std::cout << "===================" << std::endl;

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