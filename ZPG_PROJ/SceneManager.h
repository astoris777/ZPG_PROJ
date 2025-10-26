#pragma once
#include <vector>
#include "Scene.h"
#include "Camera.h"
#include "Light.h"

class ResourceManager;
class Window;

class SceneManager
{
public:
    SceneManager(ResourceManager* resourceManager, Camera* camera, Window* window);
    ~SceneManager();

    void createScenes();
    void update(float deltaTime);
    void render();
    void switchScene(int sceneIndex);
    
    void setFOV45() { currentFOV = 45.0f; }
    void setFOV90() { currentFOV = 90.0f; }
    void setFOV130() { currentFOV = 130.0f; }
    float getCurrentFOV() const { return currentFOV; }

private:
    std::vector<Scene*> scenes;
    std::vector<Light*> lights;
    int activeScene;
    float currentFOV; 
    ResourceManager* resourceManager;
    Camera* camera;
    Window* window;

    void setupLights();
};