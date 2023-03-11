#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
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

	void init();
	void update(int deltaTime);
	void render();
	void updateRatioWindowSize(int width, int height);

	// Gettters & Setters
	int getCurrentTime() { return currentTime; };

private:
	void initShaders();
	void initPlayer();
	void initEnemies();
	void initItems();
	void updateTime(int deltatime);

private:
	TileMap *map;
	Player *player;
	vector<Enemy*> enemies;
	vector<Item*> items;
	ShaderProgram texProgram;
	
	glm::mat4 projection;
<<<<<<< HEAD
	float gameRatio = 1.28f;;
=======
	float ratioWindowSize;
	float gameRatio = 1.28f;
>>>>>>> victor

	int currentTime, remainingSeconds;
	int itemTimer;
};


#endif // _SCENE_INCLUDE

