#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"


//#define JUMP_ANGLE_STEP 4
//#define JUMP_HEIGHT 96
//#define FALL_STEP 4


enum EnemyAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

Enemy::~Enemy()
{
	if (sprite != NULL)
		delete sprite;
	if (map != NULL)
		delete map;
}

// Public functions

void Enemy::init(const glm::ivec2& tileMapPos, ShaderProgram &shaderProgram) {}

void Enemy::update(int deltaTime) {}

void Enemy::render()
{
	sprite->render();
}

void Enemy::stopwatchEnding(int currentTime)
{
	if (currentTime%100 < 50 && firstHalfSecond)
	{
		posEnemy.x += 2;
		firstHalfSecond = false;
	}
	if (currentTime%100 >= 50 && !firstHalfSecond)
	{
		posEnemy.x -= 2;
		firstHalfSecond = true;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

bool Enemy::getIsPlayerKiller()
{
	return isPlayerKiller;
}

// Getters & Setters

void Enemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

// Protected functions

bool Enemy::collisionPlayer()
{
	glm::ivec2 posPlayer = map->getPosPlayer();
	int tileSize = map->getTileSize();

	
	bool collisionX = posPlayer.x+32 >= posEnemy.x+(32-hitbox.x)+6 && posEnemy.x+32-(32-hitbox.x)-6 >= posPlayer.x;
	bool collisionY = posPlayer.y+32 >= posEnemy.y+(32-hitbox.y)+4 && posEnemy.y+32-(32-hitbox.y)-4 >= posPlayer.y;

	return collisionX && collisionY;
}




