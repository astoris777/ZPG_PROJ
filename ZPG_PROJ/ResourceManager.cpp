#include "ResourceManager.h"
#include "tree.h"
#include "bushes.h"
#include "triangle.h"
#include "sphere.h"
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
    shaders.push_back(new ShaderProgram("vertex_shader_common.glsl", "fragment_shader_lambert.glsl"));
    shaders.push_back(new ShaderProgram("vertex_shader_common.glsl", "fragment_shader_phong.glsl"));
    shaders.push_back(new ShaderProgram("vertex_shader_common.glsl", "fragment_shader_constant.glsl"));
    shaders.push_back(new ShaderProgram("vertex_shader_common.glsl", "fragment_shader_blinn.glsl"));
}

void ResourceManager::createModels()
{
    models.push_back(new VertexArray(tree, treeVertexCount, VertexArray::POSITION_NORMAL));
    models.push_back(new VertexArray(bushes, bushVertexCount, VertexArray::POSITION_NORMAL));
    models.push_back(new VertexArray(triangle, triangleVertexCount, VertexArray::POSITION_COLOR_NORMAL));
    models.push_back(new VertexArray(sphere, sphereVertexCount, VertexArray::POSITION_NORMAL));
}

void ResourceManager::attachShadersToCamera(Camera* camera) {
    for (auto shader : shaders) {
        camera->attach(shader);
    }
}