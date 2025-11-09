#include "Model.h"
#include <stdexcept>
#include <vector>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <GL/glew.h>

VertexArray* Model::loadFromFile(const char* name)
{
    const std::string inputfile = std::string("assets/") + name;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), "assets/", true)) {
        throw std::runtime_error("Failed to load OBJ file!");
    }

    size_t totalIndices = 0;
    for (const auto& shape : shapes) {
        totalIndices += shape.mesh.indices.size();
    }

    std::vector<float> vertices;
    vertices.reserve(totalIndices * 8);

    const bool hasNormals = !attrib.normals.empty();
    const bool hasTexcoords = !attrib.texcoords.empty();

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            const int vidx = 3 * index.vertex_index;
            vertices.push_back(attrib.vertices[vidx + 0]);
            vertices.push_back(attrib.vertices[vidx + 1]);
            vertices.push_back(attrib.vertices[vidx + 2]);

            if (hasNormals && index.normal_index >= 0) {
                const int nidx = 3 * index.normal_index;
                vertices.push_back(attrib.normals[nidx + 0]);
                vertices.push_back(attrib.normals[nidx + 1]);
                vertices.push_back(attrib.normals[nidx + 2]);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            if (hasTexcoords && index.texcoord_index >= 0) {
                const int tidx = 2 * index.texcoord_index;
                vertices.push_back(attrib.texcoords[tidx + 0]);
                vertices.push_back(attrib.texcoords[tidx + 1]);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }
    }

    const int vertexCount = static_cast<int>(totalIndices);

    return new VertexArray(vertices.data(), vertexCount, VertexArray::POSITION_NORMAL_UV);
}

std::vector<Material*> Model::loadMaterials(const char* name)
{
    const std::string inputfile = std::string("assets/") + name;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), "assets/", true)) {
        return {};
    }

    std::vector<Material*> loadedMaterials;

    for (const auto& mat : materials) {
        Texture* texture = nullptr;

        if (!mat.diffuse_texname.empty()) {
            std::string textureName = mat.diffuse_texname;
            size_t lastSlash = textureName.find_last_of("/\\");
            if (lastSlash != std::string::npos) {
                textureName = textureName.substr(lastSlash + 1);
            }
            
            std::string texPath = "assets/" + textureName;
            texture = new Texture(texPath.c_str());
        }

        glm::vec3 diffuse(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);

        if (texture != nullptr) {
            float diffuseSum = diffuse.r + diffuse.g + diffuse.b;
            if (diffuseSum < 0.01f) {
                diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            }
        }

        Material* material = new Material(
            glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]),
            diffuse,
            glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]),
            mat.shininess,
            texture
        );

        loadedMaterials.push_back(material);
    }

    return loadedMaterials;
}

std::vector<SubMesh> Model::loadWithMaterials(const char* name, std::vector<Material*>& outMaterials)
{
    const std::string inputfile = std::string("assets/") + name;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), "assets/", true)) {
        return {};
    }

    for (size_t i = 0; i < materials.size(); i++) {
        const auto& mat = materials[i];
        Texture* texture = nullptr;
        
        if (!mat.diffuse_texname.empty()) {
            std::string textureName = mat.diffuse_texname;
            
            size_t lastSlash = textureName.find_last_of("/\\");
            if (lastSlash != std::string::npos) {
                textureName = textureName.substr(lastSlash + 1);
            }
            
            std::string texPath = "assets/" + textureName;
            texture = new Texture(texPath.c_str());
        }

        glm::vec3 diffuse(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
        if (texture != nullptr && (diffuse.r + diffuse.g + diffuse.b) < 0.01f) {
            diffuse = glm::vec3(1.0f);
        }

        Material* material = new Material(
            glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]),
            diffuse,
            glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]),
            mat.shininess,
            texture
        );
        
        outMaterials.push_back(material);
    }

    std::map<int, std::vector<float>> verticesByMaterial;
    
    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];
            int materialId = shape.mesh.material_ids[f];
            
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                
                verticesByMaterial[materialId].push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                verticesByMaterial[materialId].push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                verticesByMaterial[materialId].push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                
                if (idx.normal_index >= 0) {
                    verticesByMaterial[materialId].push_back(attrib.normals[3 * idx.normal_index + 0]);
                    verticesByMaterial[materialId].push_back(attrib.normals[3 * idx.normal_index + 1]);
                    verticesByMaterial[materialId].push_back(attrib.normals[3 * idx.normal_index + 2]);
                } else {
                    verticesByMaterial[materialId].push_back(0.0f);
                    verticesByMaterial[materialId].push_back(0.0f);
                    verticesByMaterial[materialId].push_back(0.0f);
                }
                
                if (idx.texcoord_index >= 0) {
                    verticesByMaterial[materialId].push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    verticesByMaterial[materialId].push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                } else {
                    verticesByMaterial[materialId].push_back(0.0f);
                    verticesByMaterial[materialId].push_back(0.0f);
                }
            }
            index_offset += fv;
        }
    }

    std::vector<SubMesh> submeshes;
    for (const auto& pair : verticesByMaterial) {
        int materialIndex = pair.first;
        const std::vector<float>& vertices = pair.second;
        
        int vertexCount = static_cast<int>(vertices.size() / 8);
        
        SubMesh submesh;
        submesh.vao = new VertexArray(vertices.data(), vertexCount, VertexArray::POSITION_NORMAL_UV);
        submesh.materialIndex = materialIndex;
        submesh.vertexCount = vertexCount;
        submesh.startIndex = 0;
        
        submeshes.push_back(submesh);
    }

    return submeshes;
}