#include "SceneBuilder.h"
#include "Transform.h"
#include "TranslateTransform.h"
#include "MoveTransform.h"
#include "ScaleTransform.h"
#include "RotateTransform.h"
#include <cstdlib>
#include <ctime>
#include "Model.h"

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
			resources->getPhongShader(),
			resources->getBushModel()
		);

		bush->transform.add(new TranslateTransform(glm::vec3(bushX, bushY, bushZ)));
		bush->transform.add(new ScaleTransform(glm::vec3(0.3f)));
		bush->transform.add(new RotateTransform(randomRotation, glm::vec3(0.0f, 1.0f, 0.0f)));
		bush->setMaterial(bushMaterial);

		scene->addObject(bush);
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


Scene* SceneBuilder::createSampleScene(ResourceManager* resources)
{
	Scene* scene = new Scene();

	VertexArray* f1model = Model::loadFromFile("formula1.obj");
	RenderableObject* f1car = new RenderableObject(
		resources->getPhongShader(),
		f1model
	);

	f1car->transform.add(new TranslateTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
	f1car->transform.add(new ScaleTransform(glm::vec3(0.5f)));
	scene->addObject(f1car);

	Material* carMaterial = new Material(
		glm::vec3(0.3f, 0.3f, 0.3f),
		glm::vec3(0.8f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		64.0f
	);
	f1car->setMaterial(carMaterial);
	scene->addMaterial(carMaterial);

	Light* ambientLight = Light::createAmbient(
		glm::vec3(1.0f, 1.0f, 1.0f),
		0.3f
	);
	scene->addLight(ambientLight);

	Light* directionalLight = Light::createDirectional(
		glm::vec3(-0.3f, -1.0f, -0.5f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		0.8f
	);
	scene->addLight(directionalLight);

	Light* pointLight = Light::createPoint(
		glm::vec3(3.0f, 3.0f, 3.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		1.0f,
		0.09f,
		0.032f
	);
	scene->addLight(pointLight);

	return scene;
}


Scene* SceneBuilder::createFionaScene(ResourceManager* resources)
{
	Scene* scene = new Scene();

	VertexArray* fionaModel = Model::loadFromFile("fiona.obj");
	std::vector<Material*> fionaMaterials = Model::loadMaterials("fiona.obj");

	RenderableObject* fiona = new RenderableObject(
		resources->getPhongShader(),
		fionaModel
	);

	fiona->transform.add(new TranslateTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
	fiona->transform.add(new ScaleTransform(glm::vec3(1.0f)));

	if (!fionaMaterials.empty()) {
		fiona->setMaterial(fionaMaterials[0]);
		scene->addMaterial(fionaMaterials[0]);
	}
	else {
		Texture* fionaTexture = new Texture("assets/fiona.png");
		Material* fionaMaterial = new Material(
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			32.0f,
			fionaTexture
		);
		fiona->setMaterial(fionaMaterial);
		scene->addMaterial(fionaMaterial);
	}

	scene->addObject(fiona);

	Light* ambientLight = Light::createAmbient(
		glm::vec3(0.6f, 0.6f, 0.6f),
		0.6f
	);
	scene->addLight(ambientLight);

	Light* directionalLight = Light::createDirectional(
		glm::vec3(-0.2f, -1.0f, -0.3f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		0.9f
	);
	scene->addLight(directionalLight);

	Light* frontLight = Light::createPoint(
		glm::vec3(0.0f, 2.0f, 5.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.2f
	);
	scene->addLight(frontLight);

	return scene;
}

Scene* SceneBuilder::createAirplaneScene(ResourceManager* resources)
{
	Scene* scene = new Scene();

	std::vector<Material*> airplaneMaterials;
	std::vector<SubMesh> airplaneSubmeshes = Model::loadWithMaterials("11803_Airplane_v1_l1.obj", airplaneMaterials);

	RenderableObject* airplane = new RenderableObject(
		resources->getConstantShader(),  // ← Изменено с getPhongShader()
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
	std::vector<SubMesh> helicopterSubmeshes = Model::loadWithMaterials("Seahawk.obj", helicopterMaterials);

	RenderableObject* helicopter = new RenderableObject(
		resources->getConstantShader(),  // ← Изменено с getPhongShader()
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