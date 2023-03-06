#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Vampire.h"
#include "Game.h"

#define FALL_STEP 4

enum VampireAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

// Public functions

void Vampire::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/vampire.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(1.f, 1.0f));
	
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1.f, 1.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1.f, 1.0f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1.f, 1.0f));

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(1.f, 1.0f));
	
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1.f, 1.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1.f, 1.0f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1.f, 1.0f));

	/*
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));
	*/

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Vampire::update(int deltaTime)
{
	sprite->update(deltaTime);

	humanAspectTime += deltaTime;

	if (humanAspect && humanAspectTime >= 12000)
	{
		humanAspect = false;
		heightTransformation = posEnemy.y;
		// EN QUE Y ME HE RTASNFORMADO????
		if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)
			batMov = RightUp;
		else
			batMov = LeftUp;
	}

	if (humanAspect)
	{
		humanBehavior(deltaTime);
	}
	else
	{
		batBehavior(deltaTime);
	}
}

// Private functions

void Vampire::humanBehavior(int deltaTime)
{
	if (moveRight)
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

		posEnemy.x += 1;
		posEnemy.y += FALL_STEP;

		bool bFloorDown = map->collisionMoveDown(posEnemy + glm::ivec2(16, 0), glm::ivec2(32, 32), &posEnemy.y);
		bool bShouldMoveLeft = map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), false) || !bFloorDown;

		if (bShouldMoveLeft)
		{
			posEnemy.x -= 1;
			sprite->changeAnimation(STAND_RIGHT);
			moveRight = false;

			if (!bFloorDown)
				posEnemy.y -= FALL_STEP; // instead of falling, recover original y
		}
	}
	else
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

		posEnemy.x -= 1;
		posEnemy.y += FALL_STEP;

		bool bFloorDown = map->collisionMoveDown(posEnemy - glm::ivec2(16, 0), glm::ivec2(32, 32), &posEnemy.y);
		bool bShouldMoveRight = map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), false) || !bFloorDown;

		if (bShouldMoveRight)
		{
			posEnemy.x += 1;
			sprite->changeAnimation(STAND_LEFT);
			moveRight = true;

			if (!bFloorDown)
				posEnemy.y -= FALL_STEP; // instead of falling, recover original y
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Vampire::batBehavior(int deltaTime)
{
	if (batMov == RightUp)
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

		posEnemy.x += 1;
		posEnemy.y -= 1;

		bool bShouldMoveDown = map->collisionMoveUp(posEnemy, glm::ivec2(32, 32), &posEnemy.y, false);
		bool bShouldMoveLeft = map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), false);

		if (bShouldMoveDown && !map->collisionMoveRight(posEnemy+glm::ivec2(1,1), glm::ivec2(32, 32), false))
		{
			batMov = RightDown;
		}
		else if (bShouldMoveLeft && !map->collisionMoveUp(posEnemy + glm::ivec2(-1, -1), glm::ivec2(32, 32), &posEnemy.y-1, false))
		{
			batMov = LeftUp;
		}
	}
	else if (batMov == LeftUp)
	{
		posEnemy.x -= 1;
		posEnemy.y -= 1;

		bool bShouldMoveDown = map->collisionMoveUp(posEnemy, glm::ivec2(32, 32), &posEnemy.y, false);
		bool bShouldMoveRight = map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), false);

		if (bShouldMoveDown && !map->collisionMoveLeft(posEnemy + glm::ivec2(-1, +1), glm::ivec2(32, 32), false))
		{
			batMov = LeftDown;
		}
		else if (bShouldMoveRight && !map->collisionMoveUp(posEnemy + glm::ivec2(1, -1), glm::ivec2(32, 32), &posEnemy.y-1, false))
		{
			batMov = RightUp;
		}
	}
	else if (batMov == RightDown)
	{
		posEnemy.x += 1;
		posEnemy.y += 1;

		bool bShouldMoveUp = map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y);
		bool bShouldMoveLeft = map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), false);
		
		if (bShouldMoveUp && !map->collisionMoveRight(posEnemy + glm::ivec2(1, -1), glm::ivec2(32, 32), false))
		{
			batMov = RightUp;
			if (abs(posEnemy.y - heightTransformation) >= 32)
			{
				humanAspect = true;
				humanAspectTime = 0;
			}
		}
		else if (bShouldMoveLeft && !map->collisionMoveDown(posEnemy + glm::ivec2(-1, 1), glm::ivec2(32, 32), &posEnemy.y+1))
			batMov = LeftDown;
	}
	else if (batMov == LeftDown)
	{
		posEnemy.x -= 1;
		posEnemy.y += 1;

		bool bShouldMoveUp = map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y);
		bool bShouldMoveRight = map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), false);

		if (bShouldMoveUp && !map->collisionMoveLeft(posEnemy + glm::ivec2(-1, -1), glm::ivec2(32, 32), false))
		{
			batMov = LeftUp;
			if (abs(posEnemy.y - heightTransformation) >= 32)
			{
				humanAspect = true;
				humanAspectTime = 0;
			}
		}
		else if (bShouldMoveRight && !map->collisionMoveDown(posEnemy + glm::ivec2(1, 1), glm::ivec2(32, 32), &posEnemy.y+1))
			batMov = RightDown;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}