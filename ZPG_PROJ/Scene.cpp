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
    obj->setID(nextObjectID++);
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
    for (auto obj : objects)
    {
        obj->transform.update(deltaTime);
    }

    for (auto& binding : objectLightBindings)
    {
        glm::vec3 objectPos = binding.object->transform.getPosition();
        binding.light->setPosition(objectPos + binding.offset);
    }
}

void Scene::draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPos)
{
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    for (auto obj : objects)
    {
        if (obj->shader) {
            obj->shader->use();

            glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);

            if (!lights.empty()) {
                obj->shader->setUniform("numberOfLights", static_cast<int>(lights.size()));

                for (size_t i = 0; i < lights.size() && i < 6; ++i) {
                    lights[i]->setUniformsArray(obj->shader, static_cast<int>(i));
                }

                obj->shader->setUniform("viewPos", cameraPos);
            }

            obj->draw(projection, view);
        }
    }

    glDisable(GL_STENCIL_TEST);
}

void Scene::setSelected(int index)
{
    if (index >= 0 && index < objects.size()) {
        selectedObject = objects[index];
    } else {
        selectedObject = nullptr;
    }
}

RenderableObject* Scene::getSelected() const
{
    return selectedObject;
}

RenderableObject* Scene::getObjectByID(unsigned int id) const
{
    for (auto obj : objects) {
        if (obj->getID() == id) {
            return obj;
        }
    }
    return nullptr;
}

void Scene::removeObjectByID(unsigned int id)
{
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        if ((*it)->getID() == id) {
            
            if (selectedObject == *it) {
                selectedObject = nullptr;
            }
            
            for (auto bindIt = objectLightBindings.begin(); bindIt != objectLightBindings.end();) {
                if (bindIt->object == *it) {
                    bindIt = objectLightBindings.erase(bindIt);
                } else {
                    ++bindIt;
                }
            }
            
            delete *it;
            objects.erase(it);
            return;
        }
    }
}