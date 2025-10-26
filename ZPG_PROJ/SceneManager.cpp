#include "SceneManager.h"
#include "ResourceManager.h"
#include "SceneBuilder.h"
#include "Window.h"
#include <iostream>

SceneManager::SceneManager(ResourceManager* resourceManager, Camera* camera, Window* window)
    : activeScene(0), resourceManager(resourceManager), camera(camera), window(window)
{
}

SceneManager::~SceneManager()
{
    for (auto scene : scenes)
        delete scene;
    for (auto light : lights)
        delete light;
}

void SceneManager::setupLights()
{
    lights.push_back(new Light(glm::vec3(2.0f, 3.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f)));
    lights.push_back(new Light(glm::vec3(-2.0f, 2.0f, -1.0f), glm::vec3(0.8f, 0.8f, 1.0f), 0.5f, glm::vec3(0.8f, 0.8f, 1.0f)));
}

void SceneManager::createScenes()
{
    setupLights();

    Scene* sphereScene = SceneBuilder::createSpeheresWithLightning(resourceManager);
    Scene* forestScene = SceneBuilder::createForestScene(resourceManager);
    scenes.push_back(sphereScene);
    scenes.push_back(forestScene);

    activeScene = 0;
}

void SceneManager::update(float deltaTime)
{
    scenes[activeScene]->update(deltaTime);
}

void SceneManager::render()
{
    glm::mat4 view = camera->getViewMatrix();

    // Use dynamic aspect ratio based on current window size
    float aspectRatio = window->getAspectRatio();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    glm::vec3 cameraPos = camera->getPosition();

    scenes[activeScene]->draw(projection, view, cameraPos);
}

void SceneManager::switchScene(int sceneIndex)
{
    if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
        activeScene = sceneIndex;
    }
}