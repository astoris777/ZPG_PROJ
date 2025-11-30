#include "SceneBuilder.h"
#include "Transform.h"
#include "TranslateTransform.h"
#include "MoveTransform.h"
#include "ScaleTransform.h"
#include "RotateTransform.h"
#include <cstdlib>
#include <ctime>
#include "Model.h"
#include <iostream>
#include "CustomTransform.h"

Scene* SceneBuilder::createSolarSystemScene(ResourceManager* resources)
{
	float sunScale = 5.0f;
	Scene* scene = new Scene();

	Texture* sunTexture = new Texture("assets/SolarSystem/sun.jpg");

	Material* sunMaterial = new Material(
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		1.0f,
		sunTexture
	);
	scene->addMaterial(sunMaterial);

	VertexArray* sphereModel = resources->getSphereModel();

	RenderableObject* sun = new RenderableObject(
		resources->getConstantShader(),
		sphereModel
	);

	sun->transform.add(new ScaleTransform(glm::vec3(sunScale)));
	sun->transform.add(new RotateTransform(0.0f, glm::vec3(0.0f, 1.0f, 0.0f), 1.5f));
	sun->setMaterial(sunMaterial);
	scene->addObject(sun);

	RenderableObject* mercury = new RenderableObject(
		resources->getLambertShader(),
		sphereModel
	);
	Texture* mercuryTexture = new Texture("assets/SolarSystem/mercury.jpg");

	Material* mercuryMaterial = new Material(
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.3f, 0.3f, 0.3f),
		32.0f,
		mercuryTexture
	);

	scene->addMaterial(mercuryMaterial);
	mercury->transform.add(MoveTransform::createCircular(glm::vec3(0.0f), 10.0f, 0.4f));
	mercury->transform.add(new ScaleTransform(glm::vec3(sunScale / 7.0f)));
	mercury->transform.add(new RotateTransform(0.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.8f));
	mercury->setMaterial(mercuryMaterial);
	scene->addObject(mercury);

	Texture* earthTexture = new Texture("assets/SolarSystem/earth.jpg");
	Material* earthMaterial = new Material(
		glm::vec3(0.2f, 0.3f, 0.8f),
		glm::vec3(0.2f, 0.3f, 0.8f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f,
		earthTexture
	);

	scene->addMaterial(earthMaterial);
	RenderableObject* earth = new RenderableObject(
		resources->getPhongShader(),
		sphereModel
	);


	earth->transform.add(MoveTransform::createCircular(glm::vec3(0.0f), 12.0f, 0.1f));
	earth->transform.add(new ScaleTransform(glm::vec3(sunScale / 3)));
	earth->transform.add(new RotateTransform(0.0f, glm::vec3(0.0f, 1.0f, 0.0f), 50.f));
	earth->setMaterial(earthMaterial);

	scene->addObject(earth);

	Texture* moonTexture = new Texture("assets/SolarSystem/moon.jpg");
	Material* moonMaterial = new Material(
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(0.3f, 0.3f, 0.3f),
		16.0f,
		moonTexture
	);

	scene->addMaterial(moonMaterial);
	RenderableObject* moon = new RenderableObject(
		resources->getPhongShader(),
		sphereModel
	);


	moon->transform.add(MoveTransform::createCircular(glm::vec3(0.0f), 12.0f, 0.1f));
	moon->transform.add(MoveTransform::createCircular(glm::vec3(0.0f), 4.0f, 2.0f));
	moon->transform.add(new ScaleTransform(glm::vec3(sunScale / 12)));
	moon->transform.add(new RotateTransform(0.0f, glm::vec3(0.0f, 1.0f, 0.0f), 50.f));


	moon->setMaterial(moonMaterial);
	scene->addObject(moon);

	Texture* marsTexture = new Texture("assets/SolarSystem/mars.jpg");
	Material* marsMaterial = new Material(
		glm::vec3(0.8f, 0.4f, 0.3f),
		glm::vec3(0.8f, 0.4f, 0.3f),
		glm::vec3(0.3f, 0.3f, 0.3f),
		32.0f,
		marsTexture
	);
	scene->addMaterial(marsMaterial);	

	RenderableObject* mars = new RenderableObject(
		resources->getPhongShader(),
		sphereModel
	);

	mars->transform.add(MoveTransform::createCircular(glm::vec3(0.0f), 38.0f, 0.053f));
	mars->transform.add(new ScaleTransform(glm::vec3(sunScale / 4.5f)));
	mars->transform.add(new RotateTransform(0.0f, glm::vec3(0.0f, 1.0f, 0.0f), 48.0f));
	mars->setMaterial(marsMaterial);
	scene->addObject(mars);

	Light* sunLight = Light::createPoint(
		glm::vec3(0.0f, 0.0f, 0.0f),                 
		glm::vec3(1.0f, 0.95f, 0.8f),               
		5.0f,                                    
		1.0f,                                    
		0.022f,                                   
		0.0019f                                    
	);
	scene->addLight(sunLight);

	return scene;
}


Scene* SceneBuilder::createForestScene(ResourceManager* resources)
{
	Scene* scene = new Scene();

	int treeCount = 50;
	int bushCount = 30;

	Material* treeMaterial = new Material(
		glm::vec3(0.4f, 0.3f, 0.2f),
		glm::vec3(0.6f, 0.4f, 0.2f),
		glm::vec3(0.3f, 0.3f, 0.3f),
		32.0f
	);

	Material* bushMaterial = new Material(
		glm::vec3(0.1f, 0.2f, 0.15f),
		glm::vec3(0.2f, 0.6f, 0.3f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		16.0f
	);

	scene->addMaterial(treeMaterial);
	scene->addMaterial(bushMaterial);


	VertexArray* planeModel = resources->gePlaneModel();

	Texture* grassTexture = new Texture("assets/grass.png");
	Material* grassMaterial = new Material(
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		1.0f,
		grassTexture
	);
	scene->addMaterial(grassMaterial);

	RenderableObject* grass = new RenderableObject(
		resources->getConstantShader(),
		planeModel
	);
	grass->transform.add(new ScaleTransform(glm::vec3(20.0f, 1.0f, 20.0f)));
	grass->setMaterial(grassMaterial);
	scene->addObject(grass);



	for (int i = 0; i < treeCount; i++)
	{
		float treeX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 20.0f - 10.0f;
		float treeY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.1f;
		float treeZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 20.0f - 10.0f;
		float randomRotation = static_cast<float>(rand() % 360);

		RenderableObject* tree = new RenderableObject(
			resources->getPhongShader(),
			resources->getTreeModel()
		);

		tree->transform.add(new TranslateTransform(glm::vec3(treeX, treeY, treeZ)));
		tree->transform.add(new ScaleTransform(glm::vec3(0.5f)));
		tree->transform.add(new RotateTransform(randomRotation, glm::vec3(0.0f, 1.0f, 0.0f)));
		tree->setMaterial(treeMaterial);

		scene->addObject(tree);
	}

	for (int i = 0; i < bushCount; i++)
	{
		float bushX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 15.0f - 7.5f;
		float bushY = 0.0f;
		float bushZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 15.0f - 7.5f;
		float randomRotation = static_cast<float>(rand() % 360);

		RenderableObject* bush = new RenderableObject(
			resources->getConstantShader(),
			resources->getBushModel()
		);

		bush->transform.add(new TranslateTransform(glm::vec3(bushX, bushY, bushZ)));
		bush->transform.add(new ScaleTransform(glm::vec3(0.3f)));
		bush->transform.add(new RotateTransform(randomRotation, glm::vec3(0.0f, 1.0f, 0.0f)));
		bush->setMaterial(bushMaterial);

		scene->addObject(bush);
	}

	std::vector<Material*> fionaMaterials;
	std::vector<SubMesh>fionaModel;
	resources->loadFionaModel(fionaMaterials, fionaModel);

	RenderableObject* fiona = new RenderableObject(
		resources->getPhongShader(),
		fionaModel,
		fionaMaterials
	);

	fiona->transform.add(new TranslateTransform(glm::vec3(0.0f, 0.0f, -5.0f)));
	fiona->transform.add(new CustomTransform(15.0f));
	scene->addObject(fiona);
	for (auto* mat : fionaMaterials) {
		scene->addMaterial(mat);
	}

	std::vector<Material*> shrekMaterials;
	std::vector<SubMesh>shrekModel;
	resources->loadShrekModel(shrekMaterials, shrekModel);

	RenderableObject* shrek = new RenderableObject(
		resources->getPhongShader(),
		shrekModel,
		shrekMaterials
	);

	shrek->transform.add(new TranslateTransform(glm::vec3(2.0f, 0.0f, -5.0f)));
	scene->addObject(shrek);
	for (auto* mat : shrekMaterials) {
		scene->addMaterial(mat);
	}

	Light* ambientLight = Light::createAmbient(
		glm::vec3(0.3f, 0.3f, 0.25f),
		0.4f
	);
	scene->addLight(ambientLight);

	Light* sunLight = Light::createDirectional(
		glm::vec3(-0.3f, -1.0f, -0.4f),
		glm::vec3(1.0f, 0.95f, 0.8f),
		0.9f
	);
	scene->addLight(sunLight);

	return scene;
}

Scene* SceneBuilder::createAirplaneScene(ResourceManager* resources)
{
	Scene* scene = new Scene();

	std::vector<Material*> airplaneMaterials;
	std::vector<SubMesh> airplaneSubmeshes;
	resources->loadAirplaneModel(airplaneMaterials, airplaneSubmeshes);

	RenderableObject* airplane = new RenderableObject(
		resources->getConstantShader(),
		airplaneSubmeshes,
		airplaneMaterials
	);

	airplane->transform.add(new ScaleTransform(glm::vec3(0.01f)));
	airplane->transform.add(new TranslateTransform(glm::vec3(0.0f, 0.0f, -50.0f)));
	airplane->transform.add(new RotateTransform(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));

	scene->addObject(airplane);

	for (auto* mat : airplaneMaterials) {
		scene->addMaterial(mat);
	}

	std::vector<Material*> helicopterMaterials;
	std::vector<SubMesh> helicopterSubmeshes;
	resources->loadHelicopterModel(helicopterMaterials, helicopterSubmeshes);

	RenderableObject* helicopter = new RenderableObject(
		resources->getConstantShader(),
		helicopterSubmeshes,
		helicopterMaterials
	);

	helicopter->transform.add(new ScaleTransform(glm::vec3(0.1f)));
	helicopter->transform.add(new TranslateTransform(glm::vec3(10.0f, -20.0f, -200.0f)));
	scene->addObject(helicopter);
	for (auto* mat : helicopterMaterials) {
		scene->addMaterial(mat);
	}

	return scene;
}

Scene* SceneBuilder::createFormulaOneScene(ResourceManager* resources)
{
	Scene* scene = new Scene();

	return scene;
}