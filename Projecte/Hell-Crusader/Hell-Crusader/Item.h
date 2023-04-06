#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Item is basically a Sprite that represents the wnwmy. As such it has
// all properties it needs to track its movement, and collisions.

class Item
{

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();

	bool isTaken() { return taken; };
	//bool isTreasure() { return bIsTreasure; }
	//bool isPickup() { return bIsPickup; }
	int getType() { return type; }

	// Getters & Setters
	glm::ivec2 getPosition() { return posItem; };
	bool getShowing() { return showing; };
	bool getTaken() { return taken; };

	void setTileMap(TileMap* tileMap) { map = tileMap; };
	void setPosition(const glm::vec2& pos);
	void setShowing(bool b) { showing = b; }

	bool collisionPlayer();

protected:
	glm::ivec2 spriteSize;
	Texture spritesheet;
	Sprite* sprite;

	glm::ivec2 tileMapDispl, posItem, hitbox, itemSize;
	TileMap* map;

	bool taken = false;
	bool showing = false;

	//bool bIsTreasure = false;
	//bool bIsPickup = false;

	int type = 0;
};


#endif // _ITEM_INCLUDE


