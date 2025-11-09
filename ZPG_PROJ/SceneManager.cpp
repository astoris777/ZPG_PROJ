#include "SceneManager.h"
#include "ResourceManager.h"
#include "SceneBuilder.h"
#include "Window.h"

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

void SceneManager::createScenes()
{
    Scene* airplaneScene = SceneBuilder::createAirplaneScene(resourceManager);
    Scene* forestScene = SceneBuilder::createForestScene(resourceManager);
    Scene* sampleScene = SceneBuilder::createSampleScene(resourceManager);
    Scene* fionaScene = SceneBuilder::createFionaScene(resourceManager);
    
    scenes.push_back(airplaneScene);
    scenes.push_back(fionaScene);
    scenes.push_back(sampleScene);
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
}

void SceneManager::switchScene(int sceneIndex)
{
    if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
        activeScene = sceneIndex;
    }
}