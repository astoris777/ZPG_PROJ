#include "Model.h"
#include <iostream>
#include <stdexcept>
#include <vector>

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
        if (!warn.empty()) std::cout << "Warn: " << warn << std::endl;
        if (!err.empty()) std::cerr << "Err: " << err << std::endl;
        throw std::runtime_error("Failed to load OBJ file!");
    }

    if (!warn.empty()) std::cout << "Warn: " << warn << std::endl;

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

    std::cout << "=== Loading materials from " << inputfile << " ===" << std::endl;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), "assets/", true)) {
        std::cerr << "? Failed to load OBJ materials: " << err << std::endl;
        return {};
    }

    if (!warn.empty()) {
        std::cout << "?? Warning: " << warn << std::endl;
    }

    std::vector<Material*> loadedMaterials;

    std::cout << "Found " << materials.size() << " material(s)" << std::endl;

    for (const auto& mat : materials) {
        Texture* texture = nullptr;

        std::cout << "  Material: " << mat.name << std::endl;
        std::cout << "    Ka: " << mat.ambient[0] << ", " << mat.ambient[1] << ", " << mat.ambient[2] << std::endl;
        std::cout << "    Kd: " << mat.diffuse[0] << ", " << mat.diffuse[1] << ", " << mat.diffuse[2] << std::endl;

        if (!mat.diffuse_texname.empty()) {
            std::string texPath = "assets/" + mat.diffuse_texname;
            std::cout << "    Texture: " << texPath << std::endl;
            texture = new Texture(texPath.c_str());
        }

        // ? ????: ???? Kd = (0,0,0) ?? ???? ????????, ????????? Kd = (1,1,1)
        glm::vec3 diffuse(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);

        if (texture != nullptr) {
            float diffuseSum = diffuse.r + diffuse.g + diffuse.b;
            if (diffuseSum < 0.01f) {  // ??????????? ??????
                std::cout << "    ?? Kd is black but texture exists. Using white Kd." << std::endl;
                diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            }
        }

        Material* material = new Material(
            glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]),
            diffuse,  // ? ?????????? ???????????? ????????
            glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]),
            mat.shininess,
            texture
        );

        std::cout << "  ? Created material with Kd=("
            << diffuse.r << "," << diffuse.g << "," << diffuse.b << ")" << std::endl;

        loadedMaterials.push_back(material);
    }

    return loadedMaterials;
}