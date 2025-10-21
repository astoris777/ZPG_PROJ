#include "RenderableObject.h"

RenderableObject::RenderableObject(Shader* shader, VertexArray* vertexArray)
    : shader(shader), vao(vertexArray), material(nullptr)
{
}

RenderableObject::~RenderableObject()
{
}

void RenderableObject::draw(const glm::mat4& projection, const glm::mat4& view)
{
    shader->use();

    glm::mat4 model = transform.getModelMatrix();

    shader->setUniform("projectionMatrix", projection);
    shader->setUniform("modelMatrix", model);
    shader->setUniform("viewMatrix", view);

    if (material) {
        material->setUniforms(shader);
    }

    vao->draw();
}

void RenderableObject::setMaterial(Material* mat)
{
    material = mat;
}