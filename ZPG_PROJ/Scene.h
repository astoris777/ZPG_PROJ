#pragma once
#include <vector>
#include "RenderableObject.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Material.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void addObject(RenderableObject* obj);
    void update(float deltaTime);
    void draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos);
    void addLight(Light* light);
    void addMaterial(Material* material);

private:
    std::vector<RenderableObject*> objects;
    std::vector<Light*> lights;
    std::vector<Material*> materials;
};