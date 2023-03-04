#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Skeleton.h"
#include "Item.h"
#include "Key.h"
#include "Door.h"


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
	float getCurrentTime();

	void updateWindow(glm::vec2 w);

private:
	void initShaders();
	void initPlayer();
	void initEnemies();
	void initItems();

private:
	TileMap *map;
	Player *player;
	vector<Enemy*> enemies;
	vector<Item*> items;
	ShaderProgram texProgram;
	float currentTime;
	int remainingSeconds;
	glm::mat4 projection;

	glm::vec2 windowSize;
	glm::ivec2 screenSize;

};


#endif // _SCENE_INCLUDE

