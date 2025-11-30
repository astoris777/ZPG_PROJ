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
    
    for (auto& pair : complexModelsGeometry) {
        for (auto& submesh : pair.second) {
            delete submesh.vao;
        }
    }
    
    for (auto& pair : complexModelsMaterials) {
        for (auto* material : pair.second) {
            delete material;
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
    
    std::vector<Material*> materials;
    
    complexModelsGeometry["fiona"] = Model::loadWithMaterials("fiona.obj", materials);
    complexModelsMaterials["fiona"] = materials;
    materials.clear();
    
    complexModelsGeometry["shrek"] = Model::loadWithMaterials("shrek.obj", materials);
    complexModelsMaterials["shrek"] = materials;
    materials.clear();
    
    complexModelsGeometry["airplane"] = Model::loadWithMaterials("11803_Airplane_v1_l1.obj", materials);
    complexModelsMaterials["airplane"] = materials;
    materials.clear();
    
    complexModelsGeometry["helicopter"] = Model::loadWithMaterials("Seahawk.obj", materials);
    complexModelsMaterials["helicopter"] = materials;
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
    outModel = complexModelsGeometry["fiona"];
    outMaterials.clear();
    for (auto* mat : complexModelsMaterials["fiona"]) {
        outMaterials.push_back(new Material(mat->ambient, mat->diffuse, mat->specular, mat->shininess, mat->texture));
    }
}

void ResourceManager::loadShrekModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    outModel = complexModelsGeometry["shrek"];
    outMaterials.clear();
    for (auto* mat : complexModelsMaterials["shrek"]) {
        outMaterials.push_back(new Material(mat->ambient, mat->diffuse, mat->specular, mat->shininess, mat->texture));
    }
}

void ResourceManager::loadAirplaneModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    outModel = complexModelsGeometry["airplane"];
    outMaterials.clear();
    for (auto* mat : complexModelsMaterials["airplane"]) {
        outMaterials.push_back(new Material(mat->ambient, mat->diffuse, mat->specular, mat->shininess, mat->texture));
    }
}

void ResourceManager::loadHelicopterModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    outModel = complexModelsGeometry["helicopter"];
    outMaterials.clear();
    for (auto* mat : complexModelsMaterials["helicopter"]) {
        outMaterials.push_back(new Material(mat->ambient, mat->diffuse, mat->specular, mat->shininess, mat->texture));
    }
}