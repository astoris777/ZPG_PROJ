#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "ResourceManager.h"

class Scene;

enum class TargetType {
    MINION_EASY = 1,
    SHREK_MEDIUM = 5,
    FIONA_HARD = 10
};

struct TargetInfo {
    unsigned int objectID;
    TargetType type;
    float lifetime;
    float maxLifetime;
};

class GameManager
{
public:
    GameManager(Scene* scene, ResourceManager* resources);
    ~GameManager();

    void update(float deltaTime);
    void onTargetHit(unsigned int objectID);
    
    int getScore() const { return score; }
    float getTimeRemaining() const { return gameTime; }
    bool isGameOver() const { return gameTime <= 0.0f && gameActive; }
    bool isGameActive() const { return gameActive; }
    
    void startGame(float duration = 60.0f);
    void endGame();
    void resetGame();

private:
    void spawnTarget();
    void spawnMinion();
    void spawnShrek();
    void spawnFiona();
    void removeTarget(unsigned int objectID);
    int getTargetPoints(unsigned int objectID);
    
    Scene* scene;
    ResourceManager* resources;
    
    std::vector<TargetInfo> activeTargets;
    
    int score;
    float gameTime;
    float spawnTimer;
    float spawnInterval;
    int maxTargets;
    bool gameActive;
    
    float playAreaWidth;
    float playAreaHeight;
    float playAreaMargin;
};

