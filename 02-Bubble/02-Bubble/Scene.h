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
#include "Item.h"
#include "Key.h"
#include "Door.h"
#include "Hourglass.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	// Gettters & Setters
	int getCurrentTime() { return currentTime; };
	bool getLevelFinished() { return bLevelFinished; };

private:
	void initPlayer();
	void initEnemies();
	void initItems();
	void updateTime(int deltatime);

private:
	LevelInterface* levelInterface;
	TileMap *map;
	Player *player;
	vector<Enemy*> enemies;
	vector<Item*> items;
	ShaderProgram texProgram;

	glm::ivec2 initPosPlayer;

	int currentTime, remainingSeconds = 60;
	int timer;
	int hourglassTimer;

	bool bLevelFinished;
};


#endif // _SCENE_INCLUDE

