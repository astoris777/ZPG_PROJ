#include "SceneManager.h"
#include "ResourceManager.h"
#include "SceneBuilder.h"
#include "Window.h"
#include <iostream>

SceneManager::SceneManager(ResourceManager* resourceManager, Camera* camera, Window* window)
    : activeScene(0), currentFOV(45.0f), resourceManager(resourceManager), camera(camera), window(window)
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
    float aspectRatio = window->getAspectRatio();
    glm::mat4 projection = glm::perspective(glm::radians(currentFOV), aspectRatio, 0.1f, 100.0f);
    
    camera->notify(projection);
    
    glm::mat4 view = camera->getViewMatrix();
    glm::vec3 cameraPos = camera->getPosition();

    scenes[activeScene]->draw(projection, view, cameraPos);
    
    static float lastFOV = -1.0f;
    if (lastFOV != currentFOV) {
        std::cout << "Current FOV: " << currentFOV << std::endl;
        lastFOV = currentFOV;
    }
}

void SceneManager::switchScene(int sceneIndex)
{
    if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
        activeScene = sceneIndex;
    }
}