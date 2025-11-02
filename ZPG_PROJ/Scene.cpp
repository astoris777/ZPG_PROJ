#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
    for (auto obj : objects)
        delete obj;
    for (auto light : lights)
        delete light;
    for (auto material : materials)
        delete material;
}

void Scene::addObject(RenderableObject* obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light* light)
{
    lights.push_back(light);
}

void Scene::addMaterial(Material* material)
{
    materials.push_back(material);
}

void Scene::bindObjectToLight(RenderableObject* obj, Light* light, const glm::vec3& offset)
{
    objectLightBindings.push_back({ obj, light, offset });
}

void Scene::update(float deltaTime)
{
    // ???????? ????????????? ????????
    for (auto obj : objects)
    {
        obj->transform.update(deltaTime);
    }

    // ???????? ??????? ????????? ??????
    for (auto& binding : objectLightBindings)
    {
        glm::vec3 objectPos = binding.object->transform.getPosition();
        binding.light->setPosition(objectPos + binding.offset);
    }
}

void Scene::draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos)
{
    for (auto obj : objects)
    {
        if (!lights.empty() && obj->shader) {
            obj->shader->use();

            obj->shader->setUniform("numberOfLights", static_cast<int>(lights.size()));

            for (size_t i = 0; i < lights.size() && i < 4; ++i) {
                lights[i]->setUniformsArray(obj->shader, static_cast<int>(i));
            }

            obj->shader->setUniform("viewPos", cameraPos);
        }

        obj->draw(projection, view);
    }
}