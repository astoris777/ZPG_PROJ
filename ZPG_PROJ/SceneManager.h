#pragma once
#include "Scene.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Window.h"
#include "GameManager.h"
#include <vector>

class SceneManager
{
public:
    SceneManager(ResourceManager* rm, Camera* cam, Window* win);
    ~SceneManager();

    void createScenes();
    void switchScene(int index);
    void update(float deltaTime);
    void render();
    void setSelectedObject(unsigned int stencilID);
    void deleteSelectedObject();
    void setFOV45();
    void setFOV90();
    void setFOV130();
    float getCurrentFOV() const;
    void startGame();
    void restartGame();

private:
    std::vector<Scene*> scenes;
    Scene* currentScene;
    ResourceManager* resourceManager;
    Camera* camera;
    Window* window;
    GameManager* gameManager;
    bool gameManagerActive;
    float currentFOV;
};