#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "ResourceManager.h"

class SceneBuilder {
public:
    static Scene* createForestScene(ResourceManager* resources);
	static Scene* createSampleScene(ResourceManager* resources);	
	static Scene* createFionaScene(ResourceManager* resources);
	static Scene* createAirplaneScene(ResourceManager* resources);
};