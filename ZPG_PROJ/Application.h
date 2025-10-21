#pragma once
#include "Window.h"
#include "SceneManager.h"
#include "ResourceManager.h"

class Application
{
public:
    Application();
    ~Application();

    void initialization(int width, int height);
    void run();

private:
    void processInput(float deltaTime);

    Window* window;
    SceneManager* sceneManager;
    ResourceManager* resourceManager;
};