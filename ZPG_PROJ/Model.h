#pragma once
#include "VertexArray.h"
#include "Material.h"
#include "Texture.h"
#include <string>
#include <vector>
#include <map>

struct SubMesh {
    VertexArray* vao;
    int materialIndex;
    int vertexCount;
    int startIndex;
};

class Model
{
public:
    static VertexArray* loadFromFile(const char* filename);
    static std::vector<Material*> loadMaterials(const char* filename);
    static std::vector<SubMesh> loadWithMaterials(const char* filename, std::vector<Material*>& outMaterials);

private:
    Model() = delete;
};