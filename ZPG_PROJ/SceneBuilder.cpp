#include "SceneBuilder.h"
#include "Transform.h"
#include "TranslateTransform.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Scene* SceneBuilder::createSpeheresWithLightning(ResourceManager* resources)
{
    Scene* scene = new Scene();

    ShaderProgram* phongShader = resources->getPhongShader();
    VertexArray* sphereModel = resources->getSphereModel();

    Material* sphereMaterial = new Material(glm::vec3(1.0f, 0.0f, 0.0f));

    float distance = 2.0f;
    float depth = -5.0f;

    RenderableObject* sphere1 = new RenderableObject(phongShader, sphereModel);
    sphere1->transform.add(new TranslateTransform(glm::vec3(0.0f, distance, depth)));
    sphere1->setMaterial(sphereMaterial);

    RenderableObject* sphere2 = new RenderableObject(phongShader, sphereModel);
    sphere2->transform.add(new TranslateTransform(glm::vec3(0.0f, -distance, depth)));
    sphere2->setMaterial(sphereMaterial);

    RenderableObject* sphere3 = new RenderableObject(phongShader, sphereModel);
    sphere3->transform.add(new TranslateTransform(glm::vec3(-distance, 0.0f, depth)));
    sphere3->setMaterial(sphereMaterial);

    RenderableObject* sphere4 = new RenderableObject(phongShader, sphereModel);
    sphere4->transform.add(new TranslateTransform(glm::vec3(distance, 0.0f, depth)));
    sphere4->setMaterial(sphereMaterial);

    Light* light1 = new Light(
        glm::vec3(0.0f, 0.0f, -5.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    scene->addLight(light1);

    Light* light2 = new Light(
        glm::vec3(5.0f, 5.0f, -3.0f),
        glm::vec3(1.0f, 0.5f, 0.5f),
        0.8f,
        glm::vec3(1.0f, 0.5f, 0.5f)
    );
    //scene->addLight(light2);

    scene->addMaterial(sphereMaterial);

    std::cout << "=== Scene Setup ===" << std::endl;
    std::cout << "Light1 position: (" << light1->position.x << ", " << light1->position.y << ", " << light1->position.z << ")" << std::endl;
    std::cout << "Light1 color: (" << light1->color.r << ", " << light1->color.g << ", " << light1->color.b << ")" << std::endl;
    std::cout << "Light2 position: (" << light2->position.x << ", " << light2->position.y << ", " << light2->position.z << ")" << std::endl;
    std::cout << "Light2 color: (" << light2->color.r << ", " << light2->color.g << ", " << light2->color.b << ")" << std::endl;
    std::cout << "Material color: (" << sphereMaterial->color.r << ", " << sphereMaterial->color.g << ", " << sphereMaterial->color.b << ")" << std::endl;

    scene->addObject(sphere1);
    scene->addObject(sphere2);
    scene->addObject(sphere3);
    scene->addObject(sphere4);

    return scene;
}

Scene* SceneBuilder::createForestScene(ResourceManager* resources)
{
    Scene* scene = new Scene();

    ShaderProgram* phongShader = resources->getPhongShader();

    VertexArray* treeModel = resources->getTreeModel();
    VertexArray* bushModel = resources->getBushModel();

    Material* treeMaterial = new Material(glm::vec3(0.4f, 0.3f, 0.2f));
    Material* bushMaterial = new Material(glm::vec3(0.2f, 0.6f, 0.3f));

    Light* sunLight = new Light(
        glm::vec3(10.0f, 20.0f, 10.0f),
        glm::vec3(1.0f, 1.0f, 0.1f),
        2.0f
    );
    scene->addLight(sunLight);
    scene->addMaterial(treeMaterial);
    scene->addMaterial(bushMaterial);

    srand(static_cast<unsigned int>(time(0)));

    auto getRandom = [](float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
        };

    auto generatePositions = [&](int count, float range) {
        std::vector<glm::vec3> positions;
        for (int i = 0; i < count; ++i) {
            float x = getRandom(-range, range);
            float z = getRandom(-range, range);
            positions.emplace_back(x, 0.0f, z);
        }
        return positions;
        };

    auto treePositions = generatePositions(50, 50.0f);
    auto bushPositions = generatePositions(50, 50.0f);

    for (auto& pos : treePositions) {
        RenderableObject* tree = new RenderableObject(phongShader, treeModel);
        tree->transform.add(new TranslateTransform(pos));
        tree->setMaterial(treeMaterial);
        scene->addObject(tree);
    }

    for (auto& pos : bushPositions) {
        RenderableObject* bush = new RenderableObject(phongShader, bushModel);
        bush->transform.add(new TranslateTransform(pos));
        bush->setMaterial(bushMaterial);
        scene->addObject(bush);
    }

    return scene;

}