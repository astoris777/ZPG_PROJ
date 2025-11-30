#include "ResourceManager.h"
#include "tree.h"
#include "bushes.h"
#include "triangle.h"
#include "sphere.h"
#include "Camera.h"
#include "plane.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for (auto shader : shaders)
        delete shader;
    
    for (auto& pair : simpleModels)
        delete pair.second;
    
    for (auto& pair : cachedComplexModels) {
        for (auto& submesh : pair.second) {
            delete submesh.vao;
        }
    }
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
    simpleModels.insert({"tree", new VertexArray(tree, treeVertexCount, VertexArray::POSITION_NORMAL)});
    simpleModels.insert({"bush", new VertexArray(bushes, bushVertexCount, VertexArray::POSITION_NORMAL)});
    simpleModels.insert({"sphere", Model::loadFromFile("assets/SolarSystem/Sphere.obj")});
    simpleModels.insert({"plane", new VertexArray(planeVertices, 6, VertexArray::POSITION_NORMAL_UV)});
}

void ResourceManager::attachShadersToCamera(Camera *camera)
{
    for (auto shader : shaders)
    {
        camera->attach(shader);
    }
}

VertexArray *ResourceManager::getTreeModel()
{
    return simpleModels["tree"];
}

VertexArray *ResourceManager::getBushModel()
{
    return simpleModels["bush"];
}

VertexArray *ResourceManager::getSphereModel()
{
    return simpleModels["sphere"];
}

VertexArray *ResourceManager::getPlaneModel()
{

    return simpleModels["plane"];
}

void ResourceManager::loadFionaModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    loadComplexModel("fiona", "fiona.obj", outMaterials, outModel);
}

void ResourceManager::loadShrekModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    loadComplexModel("shrek", "shrek.obj", outMaterials, outModel);
}

void ResourceManager::loadAirplaneModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    loadComplexModel("airplane", "11803_Airplane_v1_l1.obj", outMaterials, outModel);
}

void ResourceManager::loadHelicopterModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    loadComplexModel("helicopter", "Seahawk.obj", outMaterials, outModel);
}

void ResourceManager::loadComplexModel(const std::string& key, const char* filename,
                                      std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel)
{
    if (cachedComplexModels.find(key) == cachedComplexModels.end()) {
        std::vector<Material*> tempMaterials;
        cachedComplexModels[key] = Model::loadWithMaterials(filename, tempMaterials);
    }
    
    outModel = cachedComplexModels[key];
    outMaterials = Model::loadMaterials(filename);
}