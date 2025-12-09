#include "GameManager.h"
#include "Scene.h"
#include "Model.h"
#include "ScaleTransform.h"
#include "MoveTransform.h"
#include <iostream>
#include <cstdlib>

GameManager::GameManager(Scene* scene, ResourceManager* resources)
    : scene(scene), resources(resources), score(0), gameTime(0.0f),
      spawnTimer(0.0f), spawnInterval(1.5f), maxTargets(8), gameActive(false),
      playAreaWidth(20.0f), playAreaHeight(20.0f), playAreaMargin(1.5f)
{
}

GameManager::~GameManager()
{
}

void GameManager::startGame(float duration)
{
    gameActive = true;
    gameTime = duration;
    score = 0;
    spawnTimer = 0.0f;
    activeTargets.clear();
    
    for (int i = 0; i < 3; ++i) {
        spawnTarget();
    }
    
    std::cout << "========================================" << std::endl;
    std::cout << "=== WHACK-A-MOLE GAME STARTED ===" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Time: " << static_cast<int>(gameTime) << "s | Score: " << score << std::endl;
    std::cout << "Green Minions = 1 point (Easy)" << std::endl;
    std::cout << "Shrek = 5 points (Medium)" << std::endl;
    std::cout << "Fiona = 10 points (Hard)" << std::endl;
    std::cout << "========================================" << std::endl;
}

void GameManager::endGame()
{
    gameActive = false;
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "===       GAME OVER!              ===" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Final Score: " << score << " points" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Press R to restart the game" << std::endl;
    std::cout << "========================================" << std::endl;
}

void GameManager::resetGame()
{
    // Удаляем все активные цели
    for (const auto& target : activeTargets) {
        scene->removeObjectByID(target.objectID);
    }
    activeTargets.clear();
    
    // Сбрасываем счётчики
    score = 0;
    gameTime = 0.0f;
    spawnTimer = 0.0f;
    gameActive = false;
    
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "===    GAME RESTARTED!            ===" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Запускаем новую игру
    startGame();
}

void GameManager::update(float deltaTime)
{
    if (!gameActive) return;
    
    gameTime -= deltaTime;
    
    if (gameTime <= 0.0f) {
        gameTime = 0.0f;
        endGame();
        return;
    }
    
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval && activeTargets.size() < maxTargets) {
        spawnTarget();
        spawnTimer = 0.0f;
    }
    
    for (auto it = activeTargets.begin(); it != activeTargets.end();) {
        it->lifetime -= deltaTime;
        
        if (it->lifetime <= 0.0f) {
            std::cout << "Target escaped! (-1 point)" << std::endl;
            scene->removeObjectByID(it->objectID);
            it = activeTargets.erase(it);
            score = std::max(0, score - 1);
        } else {
            ++it;
        }
    }
}

void GameManager::spawnTarget()
{
    int typeRoll = rand() % 100;
    
    if (typeRoll < 60) {
        spawnMinion();
    } else if (typeRoll < 85) {
        spawnShrek();
    } else {
        spawnFiona();
    }
}

void GameManager::spawnMinion()
{
    std::vector<Material*> minionMaterials;
    std::vector<SubMesh> minionSubmeshes = Model::loadWithMaterials("Minion.obj", minionMaterials);
    
    RenderableObject* minion = new RenderableObject(
        resources->getConstantShader(),
        minionSubmeshes,
        minionMaterials
    );
    
    std::vector<glm::vec3> pathPoints;
    int numPoints = 3 + rand() % 3;
    
    float effectiveWidth = playAreaWidth - 2.0f * playAreaMargin;
    float effectiveHeight = playAreaHeight - 2.0f * playAreaMargin;
    
    for (int j = 0; j < numPoints; j++) {
        float pointX = (static_cast<float>(rand()) / RAND_MAX * effectiveWidth) - effectiveWidth / 2.0f;
        float pointZ = (static_cast<float>(rand()) / RAND_MAX * effectiveHeight) - effectiveHeight / 2.0f;
        float pointY = 2.85f + (rand() % 50) / 100.0f;
        pathPoints.push_back(glm::vec3(pointX, pointY, pointZ));
    }
    
    float speed = 2.0f + static_cast<float>(rand() % 100) / 100.0f;
    
    minion->transform.add(new ScaleTransform(glm::vec3(1.0f)));
    minion->transform.add(MoveTransform::createPolyline(pathPoints, speed, true));
    
    if (!minionMaterials.empty()) {
        minionMaterials[0]->ambient = glm::vec3(0.3f, 0.8f, 0.3f);
        minionMaterials[0]->diffuse = glm::vec3(0.4f, 1.0f, 0.4f);
        minion->setMaterial(minionMaterials[0]);
    }
    
    scene->addObject(minion);
    
    for (auto* mat : minionMaterials) {
        scene->addMaterial(mat);
    }
    
    const auto& objects = scene->getObjects();
    if (!objects.empty()) {
        unsigned int newID = objects.back()->getID();
        float lifetime = 8.0f;
        activeTargets.push_back({ newID, TargetType::MINION_EASY, lifetime, lifetime });
    }
}

void GameManager::spawnShrek()
{
    std::vector<Material*> shrekMaterials;
    std::vector<SubMesh> shrekSubmeshes = Model::loadWithMaterials("ForestScene/shrek.obj", shrekMaterials);
    
    RenderableObject* shrek = new RenderableObject(
        resources->getConstantShader(),
        shrekSubmeshes,
        shrekMaterials
    );
    
    std::vector<glm::vec3> pathPoints;
    int numPoints = 4 + rand() % 3;
    
    float effectiveWidth = playAreaWidth - 2.0f * playAreaMargin;
    float effectiveHeight = playAreaHeight - 2.0f * playAreaMargin;
    
    for (int j = 0; j < numPoints; j++) {
        float pointX = (static_cast<float>(rand()) / RAND_MAX * effectiveWidth) - effectiveWidth / 2.0f;
        float pointZ = (static_cast<float>(rand()) / RAND_MAX * effectiveHeight) - effectiveHeight / 2.0f;
        float pointY = 0.0f;
        pathPoints.push_back(glm::vec3(pointX, pointY, pointZ));
    }
    
    float speed = 3.5f + static_cast<float>(rand() % 150) / 100.0f;
    
    shrek->transform.add(new ScaleTransform(glm::vec3(0.8f)));
    shrek->transform.add(MoveTransform::createPolyline(pathPoints, speed, true));
    
    scene->addObject(shrek);
    
    for (auto* mat : shrekMaterials) {
        scene->addMaterial(mat);
    }
    
    const auto& objects = scene->getObjects();
    if (!objects.empty()) {
        unsigned int newID = objects.back()->getID();
        float lifetime = 6.0f;
        activeTargets.push_back({ newID, TargetType::SHREK_MEDIUM, lifetime, lifetime });
    }
}

void GameManager::spawnFiona()
{
    std::vector<Material*> fionaMaterials;
    std::vector<SubMesh> fionaSubmeshes = Model::loadWithMaterials("ForestScene/fiona.obj", fionaMaterials);
    
    RenderableObject* fiona = new RenderableObject(
        resources->getConstantShader(),
        fionaSubmeshes,
        fionaMaterials
    );
    
    std::vector<glm::vec3> pathPoints;
    int numPoints = 5 + rand() % 3;
    
    float effectiveWidth = playAreaWidth - 2.0f * playAreaMargin;
    float effectiveHeight = playAreaHeight - 2.0f * playAreaMargin;
    
    for (int j = 0; j < numPoints; j++) {
        float pointX = (static_cast<float>(rand()) / RAND_MAX * effectiveWidth) - effectiveWidth / 2.0f;
        float pointZ = (static_cast<float>(rand()) / RAND_MAX * effectiveHeight) - effectiveHeight / 2.0f;
        float pointY = 0.0f + (rand() % 100) / 200.0f;
        pathPoints.push_back(glm::vec3(pointX, pointY, pointZ));
    }
    
    float speed = 5.0f + static_cast<float>(rand() % 200) / 100.0f;
    
    fiona->transform.add(new ScaleTransform(glm::vec3(0.6f)));
    fiona->transform.add(MoveTransform::createPolyline(pathPoints, speed, true));
    
    scene->addObject(fiona);
    
    for (auto* mat : fionaMaterials) {
        scene->addMaterial(mat);
    }
    
    const auto& objects = scene->getObjects();
    if (!objects.empty()) {
        unsigned int newID = objects.back()->getID();
        float lifetime = 4.0f;
        activeTargets.push_back({ newID, TargetType::FIONA_HARD, lifetime, lifetime });
    }
}

void GameManager::onTargetHit(unsigned int objectID)
{
    if (!gameActive) return;
    
    int points = getTargetPoints(objectID);
    if (points > 0) {
        score += points;
        
        std::string targetName;
        if (points == 1) targetName = "MINION (Easy)";
        else if (points == 5) targetName = "SHREK (Medium)";
        else if (points == 10) targetName = "FIONA (Hard)";
        
        std::cout << "*** HIT! " << targetName << " *** +" << points 
                  << " points | Score: " << score 
                  << " | Time: " << static_cast<int>(gameTime) << "s" << std::endl;
        
        removeTarget(objectID);
        scene->removeObjectByID(objectID);
        
        if (rand() % 100 < 40) {
            spawnTarget();
        }
    }
}

void GameManager::removeTarget(unsigned int objectID)
{
    for (auto it = activeTargets.begin(); it != activeTargets.end(); ++it) {
        if (it->objectID == objectID) {
            activeTargets.erase(it);
            return;
        }
    }
}

int GameManager::getTargetPoints(unsigned int objectID)
{
    for (const auto& target : activeTargets) {
        if (target.objectID == objectID) {
            return static_cast<int>(target.type);
        }
    }
    return 0;
}
