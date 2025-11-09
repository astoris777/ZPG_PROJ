#include "RenderableObject.h"

RenderableObject::RenderableObject(Shader* shader, VertexArray* vertexArray)
    : shader(shader), vao(vertexArray), material(nullptr), useSubmeshes(false)
{
}

RenderableObject::RenderableObject(Shader* shader, const std::vector<SubMesh>& submeshes, const std::vector<Material*>& materials)
    : shader(shader), vao(nullptr), material(nullptr), submeshes(submeshes), submeshMaterials(materials), useSubmeshes(true)
{
}

RenderableObject::~RenderableObject()
{
}

void RenderableObject::draw(const glm::mat4& projection, const glm::mat4& view)
{
    glm::mat4 model = transform.getModelMatrix();

    shader->setUniform("projectionMatrix", projection);
    shader->setUniform("modelMatrix", model);
    shader->setUniform("viewMatrix", view);

    if (useSubmeshes) {
        for (size_t i = 0; i < submeshes.size(); i++) {
            const auto& submesh = submeshes[i];
            
            if (submesh.materialIndex >= 0 && submesh.materialIndex < submeshMaterials.size()) {
                Material* mat = submeshMaterials[submesh.materialIndex];
                mat->setUniforms(shader);
            }
            
            submesh.vao->draw();
        }
    }
    else {
        if (material) {
            material->setUniforms(shader);
        }
        vao->draw();
    }
}

void RenderableObject::setMaterial(Material* mat)
{
    material = mat;
}