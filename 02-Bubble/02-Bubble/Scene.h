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
	void updateWindow(glm::vec2 w);

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
	glm::vec2 windowSize;
	glm::ivec2 screenSize;

	int currentTime, remainingSeconds;
};


#endif // _SCENE_INCLUDE

