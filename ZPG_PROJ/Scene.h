#pragma once
#include <vector>
#include "RenderableObject.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Material.h"
#include "Skybox.h"

struct ObjectLightBinding
{
    RenderableObject *object;
    Light *light;
    glm::vec3 offset;
};

struct CameraSettings
{
    glm::vec3 position;
    glm::vec3 target;
    float sensitivity;
    float moveSpeed;

    CameraSettings()
        : position(0.0f, 0.0f, 3.0f),
          target(0.0f, 0.0f, 0.0f),
          sensitivity(0.1f),
          moveSpeed(0.05f) {}
};
class Scene
{
public:
    Scene();
    ~Scene();

    void addObject(RenderableObject *obj);
    void update(float deltaTime);
    void draw(const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &cameraPos, const glm::vec3& cameraDir);
    void addLight(Light *light);
    void addMaterial(Material *material);
    void bindObjectToLight(RenderableObject *obj, Light *light, const glm::vec3 &offset = glm::vec3(0.0f));
    void setSelected(int index);
    RenderableObject *getSelected() const;
    RenderableObject *getObjectByID(unsigned int id) const;
    const std::vector<RenderableObject *> &getObjects() const { return objects; }
    void removeObjectByID(unsigned int id);
    void setCameraSettings(const CameraSettings& settings);
    const CameraSettings& getCameraSettings() const;

    void setSkybox(Skybox* sb) { skybox = sb; }
    Skybox* getSkybox() const { return skybox; }


    int getScore() const { return score; }
    void resetScore() { score = 0; }

    

private:
    std::vector<RenderableObject *> objects;
    std::vector<Light *> lights;
    std::vector<Material *> materials;
    std::vector<ObjectLightBinding> objectLightBindings;
    unsigned int nextObjectID = 1;
    RenderableObject *selectedObject = nullptr;
    CameraSettings cameraSettings;
    Skybox* skybox = nullptr;
    int score = 0;
};