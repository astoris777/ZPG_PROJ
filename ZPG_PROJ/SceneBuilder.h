#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "ResourceManager.h"
#include <glm/glm.hpp>


class SceneBuilder {
public:
    static Scene* createForestScene(ResourceManager* resources);
    static Scene* createAirplaneScene(ResourceManager* resources);
    static Scene* createSolarSystemScene(ResourceManager* resources);
    static Scene* createWhackAMoleScene(ResourceManager* resources);
};