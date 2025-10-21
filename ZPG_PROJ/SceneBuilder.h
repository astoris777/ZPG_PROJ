#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "ResourceManager.h"

class SceneBuilder {
public:
    static Scene* createSpeheresWithLightning(ResourceManager* resources);
    static Scene* createForestScene(ResourceManager* resources);

};