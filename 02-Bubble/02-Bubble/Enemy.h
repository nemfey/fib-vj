#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Enemy is basically a Sprite that represents the wnwmy. As such it has
// all properties it needs to track its movement, and collisions.


class Enemy
{

public:
	~Enemy();

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();
	void stopwatchEnding(int currentTime);
	bool getIsPlayerKiller();

	// Getters & Setters
	void setPosition(const glm::vec2& pos);
	void setTileMap(TileMap* tileMap) { map = tileMap; };

	virtual bool collisionPlayer();

protected:
	Texture spritesheet;
	Sprite* sprite;

	glm::ivec2 tileMapDispl, posEnemy;
	TileMap* map;
	glm::ivec2 hitbox;

	bool firstHalfSecond = true;

	bool isPlayerKiller = false;
};


#endif // _ENEMY_INCLUDE


