#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Enemy is basically a Sprite that represents the wnwmy. As such it has
// all properties it needs to track its movement, and collisions.


class Enemy
{

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	bool collisionPlayer();

protected:
	glm::ivec2 tileMapDispl, posEnemy;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _ENEMY_INCLUDE


