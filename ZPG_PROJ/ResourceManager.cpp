#include "ResourceManager.h"
#include "assets/ForestScene/tree.h"
#include "assets/ForestScene/bushes.h"
#include "Camera.h"
#include "plane.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for (auto shader : shaders)
        delete shader;

    for (auto &pair : simpleModels)
        delete pair.second;

    for (auto &pair : complexModelsGeometry)
    {
        for (auto &submesh : pair.second)
        {
            delete submesh.vao;
        }
    }

    for (auto &pair : complexModelsMaterials)
    {
        for (auto *material : pair.second)
        {
            delete material;
        }
    }

    for (auto &pair : loadedTextures)
    {
        delete pair.second;
    }
}

void ResourceManager::createShaders()
{
    shaders.push_back(new ShaderProgram("Shaders/vertex_shader_common.glsl", "Shaders/fragment_shader_lambert.glsl"));
    shaders.push_back(new ShaderProgram("Shaders/vertex_shader_common.glsl", "Shaders/fragment_shader_phong.glsl"));
    shaders.push_back(new ShaderProgram("Shaders/vertex_shader_common.glsl", "Shaders/fragment_shader_constant.glsl"));
    shaders.push_back(new ShaderProgram("Shaders/vertex_shader_common.glsl", "Shaders/fragment_shader_blinn.glsl"));
}

void ResourceManager::createModels()
{
    simpleModels.insert({"tree", new VertexArray(tree, treeVertexCount, VertexArray::POSITION_NORMAL)});
    simpleModels.insert({"bush", new VertexArray(bushes, bushVertexCount, VertexArray::POSITION_NORMAL)});
    simpleModels.insert({"sphere", Model::loadFromFile("assets/SolarSystemScene/Sphere.obj")});
    simpleModels.insert({"plane", new VertexArray(planeVertices, 6, VertexArray::POSITION_NORMAL_UV)});
    simpleModels.insert({"login", Model::loadFromFile("assets/login/login.obj")});

    std::vector<Material *> materials;

    complexModelsGeometry["fiona"] = Model::loadWithMaterials("ForestScene/fiona.obj", materials);
    complexModelsMaterials["fiona"] = materials;
    materials.clear();

    complexModelsGeometry["shrek"] = Model::loadWithMaterials("ForestScene/shrek.obj", materials);
    complexModelsMaterials["shrek"] = materials;
    materials.clear();

    complexModelsGeometry["airplane"] = Model::loadWithMaterials("AirplaneScene/11803_Airplane_v1_l1.obj", materials);
    complexModelsMaterials["airplane"] = materials;
    materials.clear();

    complexModelsGeometry["helicopter"] = Model::loadWithMaterials("AirplaneScene/Seahawk.obj", materials);
    complexModelsMaterials["helicopter"] = materials;
    materials.clear();

}

void ResourceManager::createTextures()
{
    loadedTextures.insert({"earth", new Texture("assets/SolarSystemScene/earth.jpg")});
    loadedTextures.insert({"moon", new Texture("assets/SolarSystemScene/moon.jpg")});
    loadedTextures.insert({"mars", new Texture("assets/SolarSystemScene/mars.jpg")});
    loadedTextures.insert({"mercury", new Texture("assets/SolarSystemScene/mercury.jpg")});
    loadedTextures.insert({"sun", new Texture("assets/SolarSystemScene/sun.jpg")});
    loadedTextures.insert({"grass", new Texture("assets/ForestScene/grass.png")});

    loadedTextures.insert({"skybox_right", new Texture("assets/skybox/posx.jpg", false)});
    loadedTextures.insert({"skybox_left", new Texture("assets/skybox/negx.jpg", false)});
    loadedTextures.insert({"skybox_bottom", new Texture("assets/skybox/negy.jpg", false)});
    loadedTextures.insert({"skybox_top", new Texture("assets/skybox/posy.jpg", false)});
    loadedTextures.insert({"skybox_back", new Texture("assets/skybox/negz.jpg", false)});
    loadedTextures.insert({"skybox_front", new Texture("assets/skybox/posz.jpg", false)});

    loadedTextures.insert({"stars_right", new Texture("assets/skybox/stars.jpg", false)});
    // loadedTextures.insert({"stars_left", new Texture("assets/skybox/stars.jpg", false)});
    // loadedTextures.insert({"stars_top", new Texture("assets/skybox/stars.jpg", false)});
    // loadedTextures.insert({"stars_bottom", new Texture("assets/skybox/stars.jpg", false)});
    // loadedTextures.insert({"stars_front", new Texture("assets/skybox/stars.jpg", false)});
    // loadedTextures.insert({"stars_back", new Texture("assets/skybox/stars.jpg", false)});
}

Texture *ResourceManager::getTexture(const std::string &name)
{
    auto it = loadedTextures.find(name);
    if (it != loadedTextures.end())
    {
        return it->second;
    }
    return nullptr;
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

VertexArray * ResourceManager::getLoginModel()
{
    return simpleModels["login"];
}

void ResourceManager::loadFionaModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    outModel = complexModelsGeometry["fiona"];
    outMaterials.clear();
    for (auto *mat : complexModelsMaterials["fiona"])
    {
        outMaterials.push_back(new Material(mat->ambient, mat->diffuse, mat->specular, mat->shininess, mat->texture));
    }
}

void ResourceManager::loadShrekModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    outModel = complexModelsGeometry["shrek"];
    outMaterials.clear();
    for (auto *mat : complexModelsMaterials["shrek"])
    {
        outMaterials.push_back(new Material(mat->ambient, mat->diffuse, mat->specular, mat->shininess, mat->texture));
    }
}

void ResourceManager::loadAirplaneModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    outModel = complexModelsGeometry["airplane"];
    outMaterials.clear();
    for (auto *mat : complexModelsMaterials["airplane"])
    {
        outMaterials.push_back(new Material(mat->ambient, mat->diffuse, mat->specular, mat->shininess, mat->texture));
    }
}

void ResourceManager::loadHelicopterModel(std::vector<Material *> &outMaterials, std::vector<SubMesh> &outModel)
{
    outModel = complexModelsGeometry["helicopter"];
    outMaterials.clear();
    for (auto *mat : complexModelsMaterials["helicopter"])
    {
        outMaterials.push_back(new Material(mat->ambient, mat->diffuse, mat->specular, mat->shininess, mat->texture));
    }
}
