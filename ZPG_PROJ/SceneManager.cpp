#include "SceneManager.h"
#include "SceneBuilder.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

SceneManager::SceneManager(ResourceManager *rm, Camera *cam, Window *win)
    : resourceManager(rm), camera(cam), window(win), currentScene(nullptr), currentFOV(45.0f)
{
}

SceneManager::~SceneManager()
{
    for (auto scene : scenes)
    {
        delete scene;
    }
}

void SceneManager::createScenes()
{
    scenes.push_back(SceneBuilder::createSpheresScene(resourceManager));
    scenes.push_back(SceneBuilder::createSolarSystemScene(resourceManager));
    scenes.push_back(SceneBuilder::createForestScene(resourceManager, camera));
    scenes.push_back(SceneBuilder::createAirplaneScene(resourceManager));
    scenes.push_back(SceneBuilder::createFormulaOneScene(resourceManager));
    

    if (!scenes.empty())
    {
        currentScene = scenes[0];
        
        if (camera && currentScene) {
            const CameraSettings& settings = currentScene->getCameraSettings();
            camera->setPosition(settings.position);
            camera->lookAt(settings.target);
            camera->setSensitivity(settings.sensitivity);
            camera->setMoveSpeed(settings.moveSpeed);
        }
    }
}

void SceneManager::switchScene(int index)
{
    if (index >= 0 && index < scenes.size())
    {
        currentScene = scenes[index];
        
        if (camera && currentScene) {
            const CameraSettings& settings = currentScene->getCameraSettings();
            camera->setPosition(settings.position);
            camera->lookAt(settings.target);
            camera->setSensitivity(settings.sensitivity);
            camera->setMoveSpeed(settings.moveSpeed);
        }
    }
}

void SceneManager::update(float deltaTime)
{
    if (currentScene)
    {
        currentScene->update(deltaTime);
    }
}

void SceneManager::render()
{
    if (currentScene && camera)
    {
        float aspectRatio = static_cast<float>(window->getWidth()) / static_cast<float>(window->getHeight());
        glm::mat4 projection = glm::perspective(glm::radians(currentFOV), aspectRatio, 0.1f, 100.0f);
        glm::mat4 view = camera->getViewMatrix();
        glm::vec3 cameraPos = camera->getPosition();
        glm::vec3 cameraDir = camera->getDirection();

        currentScene->draw(projection, view, cameraPos, cameraDir);
    }
}

void SceneManager::setSelectedObject(unsigned int stencilID)
{
    if (currentScene)
    {
        RenderableObject *obj = currentScene->getObjectByID(stencilID);
        if (obj)
        {
            currentScene->setSelected(stencilID);
        }
    }
}

void SceneManager::deleteSelectedObject()
{
    if (currentScene)
    {
        RenderableObject *selected = currentScene->getSelected();
        if (selected)
        {
            unsigned int id = selected->getID();
            currentScene->removeObjectByID(id);
        }
    }
}

void SceneManager::setFOV45()
{
    currentFOV = 45.0f;
    std::cout << "FOV set to 45 degrees" << std::endl;
}

void SceneManager::setFOV90()
{
    currentFOV = 90.0f;
    std::cout << "FOV set to 90 degrees" << std::endl;
}

void SceneManager::setFOV130()
{
    currentFOV = 130.0f;
    std::cout << "FOV set to 130 degrees" << std::endl;
}

float SceneManager::getCurrentFOV() const
{
    return currentFOV;
}