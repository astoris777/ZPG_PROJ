#pragma once
#include <vector>
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Material.h"
#include "Model.h"
#include <unordered_map>

class Camera;

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void createShaders();
    void attachShadersToCamera(Camera* camera); 

    ShaderProgram* getLambertShader() const { return shaders[0]; }
    ShaderProgram* getPhongShader() const { return shaders[1]; }
    ShaderProgram* getConstantShader() const { return shaders[2]; }
    ShaderProgram* getBlinnShader() const { return shaders[3]; }

    VertexArray* getTreeModel();
    VertexArray* getBushModel();
    VertexArray* getSphereModel();
    VertexArray* getPlaneModel();
    void loadFionaModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel);
    void loadShrekModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel);

    void loadAirplaneModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel);
    void loadHelicopterModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel);

    Texture* getTexture(const std::string& name);

    const std::vector<ShaderProgram*>& getAllShaders() const { return shaders; }

    void createModels();
    void createTextures();

private:
    std::vector<ShaderProgram*> shaders;
    std::unordered_map<std::string, VertexArray*> simpleModels;
    std::unordered_map<std::string, std::vector<SubMesh>> complexModelsGeometry;
    std::unordered_map<std::string, std::vector<Material*>> complexModelsMaterials;
    std::unordered_map<std::string, Texture*> loadedTextures;
};