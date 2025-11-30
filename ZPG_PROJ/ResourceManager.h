#pragma once
#include <vector>
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Material.h"
#include "Model.h"

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
    VertexArray* gePlaneModel();
    void loadFionaModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel);
    void loadShrekModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel);

    void loadAirplaneModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel);
    void loadHelicopterModel(std::vector<Material*>& outMaterials, std::vector<SubMesh>& outModel);

    const std::vector<ShaderProgram*>& getAllShaders() const { return shaders; }

private:
    std::vector<ShaderProgram*> shaders;
    std::vector<VertexArray*> models;
};