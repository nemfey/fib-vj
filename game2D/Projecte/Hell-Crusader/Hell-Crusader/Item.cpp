#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Item.h"
#include "Game.h"


enum ItemAnims
{
	IDLE
};

// Public functions

void Item::init(const glm::ivec2& tileMapPos, ShaderProgram &shaderProgram) {}

void Item::update(int deltaTime) {}

void Item::render() {}

// Getters & Setters

void Item::setPosition(const glm::vec2& pos)
{
	posItem = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

// Protected functions

bool Item::collisionPlayer()
{
	glm::ivec2 posPlayer = map->getPosPlayer();
	int tileSize = map->getTileSize();
	
	//bool collisionX = posPlayer.x + 32 >= posItem.x && posItem.x + itemSize.x >= posPlayer.x;
	//bool collisionY = posPlayer.y + 32 >= posItem.y && posItem.y + itemSize.y >= posPlayer.y;
	bool collisionX = posPlayer.x + 32 >= posItem.x+((itemSize.x-hitbox.x)/2) && posItem.x+itemSize.x-(itemSize.x - hitbox.x)/2 >= posPlayer.x;
	bool collisionY = posPlayer.y + 32 >= posItem.y+ (itemSize.y - hitbox.y)/2 && posItem.y + itemSize.y-(itemSize.y - hitbox.y)/2 >= posPlayer.y;

	return collisionX && collisionY;
}