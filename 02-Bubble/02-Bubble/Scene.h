#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "LevelInterface.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Skeleton.h"
#include "Vampire.h"
#include "Reaper.h"
#include "PlayerKiller.h"
#include "Item.h"
#include "Key.h"
#include "Door.h"
#include "Hourglass.h"
#include "Treasure.h"

#include "StageState.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(ShaderProgram &shaderProgram, string level);
	void update(int deltaTime);
	void render();

	// Gettters & Setters
	int getCurrentTime() { return currentTime; };
	int getPlayerLives() { return player->getLives(); };
	int getPlayerScore() { return player->getScore(); };
	void setPlayerLives(int lives) { player->setLives(lives); };
	void setPlayerScore(int score) { player->addScore(score); };

private:
	void initPlayer();
	void initEnemies();
	void initItems();
	void stageClearMessage();
	void gameOverMessage();
	void updateTime(int deltaTime);
	void updatePlayer(int deltaTime);
	void updateEnemies(int deltaTime);
	void updateItems(int deltaTime);
	void updateLevelInterface(int deltaTime);

public:
	//enum State { Playing, StageCleared, GameOver };
	StageState state;

private:
	LevelInterface* levelInterface;
	TileMap *map;
	Player *player;
	vector<Enemy*> enemies;
	vector<Item*> items;
	ShaderProgram texProgram;

	glm::ivec2 initPosPlayer;

	int currentTime = 60;
	int remainingSeconds = 60;
	int timer = 0;
	int hourglassTimer = 0;
	int messageTimer = 0;

	int itemCountDown = 0;
	int itemSpawnCounter = 7 - (rand() % 5);
	bool itemSpawned = false;

	int itemRNG = rand() % 100;

	bool bDoorTaken, bPlayerDead;
	bool bHourglassEnding;
};


#endif // _SCENE_INCLUDE

