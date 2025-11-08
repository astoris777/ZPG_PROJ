#pragma once
#include "VertexArray.h"
#include "Material.h"
#include "Texture.h"
#include <string>
#include <vector>

class Model
{
public:
    static VertexArray* loadFromFile(const char* filename);
    static std::vector<Material*> loadMaterials(const char* filename);

private:
    Model() = delete;
};