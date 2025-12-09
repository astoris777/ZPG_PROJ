#include "SceneManager.h"
#include "SceneBuilder.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

SceneManager::SceneManager(ResourceManager *rm, Camera *cam, Window *win)
    : resourceManager(rm), camera(cam), window(win), currentScene(nullptr), currentFOV(45.0f),
      currentSceneIndex(0), gameSceneIndex(0)
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
    gameSceneIndex = 0;  // Игровая сцена - первая
    scenes.push_back(SceneBuilder::createGameScene(resourceManager));
    scenes.push_back(SceneBuilder::createSpheresScene(resourceManager));
    scenes.push_back(SceneBuilder::createForestScene(resourceManager, camera));
    scenes.push_back(SceneBuilder::createSolarSystemScene(resourceManager));
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
        currentSceneIndex = index;
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
        currentScene->setSelected(stencilID);
    }
}

void SceneManager::handleObjectClick(unsigned int stencilID)
{
    if (!currentScene) return;
    
    // Проверяем, это игровая сцена?
    if (currentSceneIndex == gameSceneIndex)
    {
        // Игровая сцена - удаляем сразу при клике
        GameManager* gm = currentScene->getGameManager();
        if (gm && gm->isGameActive())
        {
            std::cout << "[GAME] Hit object ID: " << stencilID << std::endl;
            gm->onTargetHit(stencilID);
        }
        else
        {
            std::cout << "[GAME] Deleting object ID: " << stencilID << std::endl;
            setSelectedObject(stencilID);
            deleteSelectedObject();
        }
    }
    else
    {
        // Обычная сцена - только выделяем и выводим сообщение
        std::cout << "[SCENE] Selected object with stencil index: " << stencilID << std::endl;
        std::cout << "[SCENE] Press DELETE key to remove this object" << std::endl;
        setSelectedObject(stencilID);
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
            std::cout << "Deleting object with ID: " << id << std::endl;
            currentScene->removeObjectByID(id);
        }
        else
        {
            std::cout << "No object selected to delete!" << std::endl;
        }
    }
}

void SceneManager::restartGame()
{
    if (currentScene && currentSceneIndex == gameSceneIndex)
    {
        GameManager* gm = currentScene->getGameManager();
        if (gm)
        {
            gm->resetGame();
        }
    }
    else
    {
        std::cout << "Can only restart in game scene (Scene 1)" << std::endl;
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