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

private:
    std::vector<Scene*> scenes;
    std::vector<Light*> lights;
    int activeScene;
    ResourceManager* resourceManager;
    Camera* camera;
    Window* window;

    void setupLights();
};