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


void ResourceManager::attachShadersToCamera(Camera* camera) {
    for (auto shader : shaders) {
        camera->attach(shader);
    }
}

VertexArray* ResourceManager::getTreeModel() 
{
    return new VertexArray(tree, treeVertexCount, VertexArray::POSITION_NORMAL);
}

VertexArray* ResourceManager::getBushModel()  
{
    return new VertexArray(bushes, bushVertexCount, VertexArray::POSITION_NORMAL);
}

VertexArray* ResourceManager::getSphereModel()  
{
    return Model::loadFromFile("assets/SolarSystem/Sphere.obj"); 
}

VertexArray* ResourceManager::gePlaneModel()  
{
    static float planeVertices[] = {
		-0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		-0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		-0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};

    return new VertexArray(planeVertices, 6, VertexArray::POSITION_NORMAL_UV);
}

void ResourceManager::loadFionaModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel) 
{

    outModel = Model::loadWithMaterials("fiona.obj", outMaterials);
}

void ResourceManager::loadShrekModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel) 
{

    outModel = Model::loadWithMaterials("shrek.obj", outMaterials);
}

void ResourceManager::loadAirplaneModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel) 
{

    outModel = Model::loadWithMaterials("11803_Airplane_v1_l1.obj", outMaterials);
}
void ResourceManager::loadHelicopterModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel) 
{

    outModel = Model::loadWithMaterials("Seahawk.obj", outMaterials);
}