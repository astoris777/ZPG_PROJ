#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "Transformations.h"
#include "Material.h"

class RenderableObject
{
public:
    RenderableObject(Shader* shader, VertexArray* vertexArray);
    ~RenderableObject();
    void draw(const glm::mat4& projection, const glm::mat4& view);
    void setMaterial(Material* mat);

    Shader* shader;
    VertexArray* vao;
    Transform transform;
    Material* material;
};