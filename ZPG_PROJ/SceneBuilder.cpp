#include "SceneBuilder.h"
#include "Transform.h"
#include "TranslateTransform.h"
#include "MoveTransform.h"
#include "ScaleTransform.h"
#include "RotateTransform.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Scene* SceneBuilder::createForestScene(ResourceManager* resources)
{
	Scene* scene = new Scene(); 

	int treeCount = 50;
	int bushCount = 30;

	// ??????? ?????????
	Material* treeMaterial = new Material(glm::vec3(0.6f, 0.4f, 0.2f)); // ???????
	Material* bushMaterial = new Material(glm::vec3(0.2f, 0.6f, 0.3f)); // ??????? ??? ??????
	
	scene->addMaterial(treeMaterial);
	scene->addMaterial(bushMaterial);

	// ??????? ???????
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

	// ??????? ?????
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

	
	// 2. ?????? (directional light) - ???????? ???????? ?????
	Light* sunLight = Light::createDirectional(
		glm::vec3(-0.3f, -1.0f, -0.2f),  // ??????????? (??????-??????)
		glm::vec3(1.0f, 0.95f, 0.8f),    // ?????? ????????? ????
		1.5f                             // ???????
	);
	//scene->addLight(sunLight);
	
	// 3. ?????????????? ??????????? ???? (point light)
	Light* fillLight = Light::createPoint(
		glm::vec3(5.0f, 10.0f, 5.0f),    // ???????
		glm::vec3(0.6f, 0.7f, 1.0f),     // ???????? ??????????? ????
		0.8f,                            // ?????????????
		1.0f, 0.1f, 0.05f               // Attenuation ?????????
	);
	//scene->addLight(fillLight);

	return scene;
}

