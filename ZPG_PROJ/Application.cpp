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
    window->inputManager->processInput(deltaTime);
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