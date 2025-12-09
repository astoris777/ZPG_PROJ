#include "Application.h"
#include <GLFW/glfw3.h>

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
    resourceManager->attachShadersToCamera(window->camera);
    resourceManager->createModels();
    resourceManager->createTextures();
    sceneManager->createScenes();
    
    window->inputManager->setSceneManager(sceneManager);
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

    window->inputManager->checkDeleteKey();

    static bool f1Pressed = false, f2Pressed = false, f3Pressed = false;
    static bool key1Pressed = false, key2Pressed = false, key3Pressed = false;
    static bool rPressed = false;

    // Рестарт игры по клавише R
    if (window->isKeyPressed(GLFW_KEY_R) && !rPressed) {
        sceneManager->restartGame();
        rPressed = true;
    } else if (!window->isKeyPressed(GLFW_KEY_R)) {
        rPressed = false;
    }

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

    if (window->isKeyPressed(GLFW_KEY_1) && !key1Pressed)
    {
        sceneManager->switchScene(0);
        key1Pressed = true;
    } else if (!window->isKeyPressed(GLFW_KEY_1)) {
        key1Pressed = false;
    }

    if (window->isKeyPressed(GLFW_KEY_2) && !key2Pressed)
    {
        sceneManager->switchScene(1);
        key2Pressed = true;
    } else if (!window->isKeyPressed(GLFW_KEY_2)) {
        key2Pressed = false;
    }

    if (window->isKeyPressed(GLFW_KEY_3) && !key3Pressed)
    {
        sceneManager->switchScene(2);
        key3Pressed = true;
    } else if (!window->isKeyPressed(GLFW_KEY_3)) {
        key3Pressed = false;
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