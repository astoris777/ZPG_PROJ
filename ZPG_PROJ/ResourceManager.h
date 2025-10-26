#pragma once
#include <vector>
#include "ShaderProgram.h"
#include "VertexArray.h"

class Camera;

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void createShaders();
    void createModels();
    void attachShadersToCamera(Camera* camera); 

    ShaderProgram* getLambertShader() const { return shaders[0]; }
    ShaderProgram* getPhongShader() const { return shaders[1]; }
    ShaderProgram* getConstantShader() const { return shaders[2]; }
    ShaderProgram* getBlinnShader() const { return shaders[3]; }

    VertexArray* getTreeModel() const { return models[0]; }
    VertexArray* getBushModel() const { return models[1]; }
    VertexArray* getTriangleModel() const { return models[2]; }
    VertexArray* getSphereModel() const { return models[3]; }

    const std::vector<ShaderProgram*>& getAllShaders() const { return shaders; }

private:
    std::vector<ShaderProgram*> shaders;
    std::vector<VertexArray*> models;
};