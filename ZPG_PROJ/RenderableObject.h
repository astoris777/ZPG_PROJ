#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "Transform.h"
#include "Material.h"
#include "Model.h"
#include <vector>

class RenderableObject
{
public:
    RenderableObject(Shader* shader, VertexArray* vertexArray);
    RenderableObject(Shader* shader, const std::vector<SubMesh>& submeshes, const std::vector<Material*>& materials);
    ~RenderableObject();

    void draw(const glm::mat4& projection, const glm::mat4& view);
    void setMaterial(Material* mat);
    
    // ?????? ??? ?????? ? ID ???????
    void setID(unsigned int id) { objectID = id; }
    unsigned int getID() const { return objectID; }

    Shader* shader;
    VertexArray* vao;
    Transform transform;
    Material* material;

    std::vector<SubMesh> submeshes;
    std::vector<Material*> submeshMaterials;
    bool useSubmeshes;

private:
    unsigned int objectID = 0;
};