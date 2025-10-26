#include "ResourceManager.h"
#include "tree.h"
#include "bushes.h"
#include "triangle.h"
#include "sphere.h"
#include <iostream>
#include "Camera.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for (auto shader : shaders)
        delete shader;
    for (auto model : models)
        delete model;
}

void ResourceManager::createShaders()
{
    std::cout << "=== CREATING ALL SHADERS ===" << std::endl;

    shaders.push_back(new ShaderProgram("vertex_shader_common.glsl", "fragment_shader_lambert.glsl"));
    shaders.push_back(new ShaderProgram("vertex_shader_common.glsl", "fragment_shader_phong.glsl"));
    shaders.push_back(new ShaderProgram("vertex_shader_common.glsl", "fragment_shader_constant.glsl"));
    shaders.push_back(new ShaderProgram("vertex_shader_common.glsl", "fragment_shader_blinn.glsl"));

    std::cout << "=== ALL SHADERS CREATED ===" << std::endl;
}




void ResourceManager::createModels()
{
    std::cout << "=== CREATING MODELS ===" << std::endl;

    models.push_back(new VertexArray(tree, treeVertexCount, VertexArray::POSITION_NORMAL));
    models.push_back(new VertexArray(bushes, bushVertexCount, VertexArray::POSITION_NORMAL));


    models.push_back(new VertexArray(triangle, triangleVertexCount, VertexArray::POSITION_COLOR_NORMAL));
    models.push_back(new VertexArray(sphere, sphereVertexCount, VertexArray::POSITION_NORMAL));

    std::cout << "=== MODELS CREATED ===" << std::endl;
}

void ResourceManager::attachShadersToCamera(Camera* camera) {
    for (auto shader : shaders) {
        camera->attach(shader);
    }
}