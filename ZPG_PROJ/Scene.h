#pragma once
#include <vector>
#include "RenderableObject.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Material.h"

struct ObjectLightBinding {
    RenderableObject* object;
    Light* light;
    glm::vec3 offset; // ???????? ????? ???????????? ???????
};

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
	void bindObjectToLight(RenderableObject* obj, Light* light, const glm::vec3& offset = glm::vec3(0.0f));    

private:
    std::vector<RenderableObject*> objects;
    std::vector<Light*> lights;
    std::vector<Material*> materials;
	std::vector<ObjectLightBinding> objectLightBindings;
};