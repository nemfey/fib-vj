#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Vampire.h"
#include "Game.h"

#define FALL_STEP 4

enum VampireAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, FLY, TRANSFORM, UNTRANSFORM
};

// Public functions

void Vampire::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/devil.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.0f));
	
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(FLY, 8);
	sprite->addKeyframe(FLY, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(FLY, glm::vec2(0.25f, 0.75f));
	sprite->addKeyframe(FLY, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(FLY, glm::vec2(0.75f, 0.75f));
	sprite->addKeyframe(FLY, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(FLY, glm::vec2(0.25f, 0.75f));
	
	sprite->setAnimationSpeed(TRANSFORM, 8);
	sprite->addKeyframe(TRANSFORM, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(TRANSFORM, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(TRANSFORM, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(TRANSFORM, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(TRANSFORM, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(TRANSFORM, glm::vec2(0.75f, 0.5f));
	
	sprite->setAnimationSpeed(UNTRANSFORM, 8);
	sprite->addKeyframe(UNTRANSFORM, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(UNTRANSFORM, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(UNTRANSFORM, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(UNTRANSFORM, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(UNTRANSFORM, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(UNTRANSFORM, glm::vec2(0.5f, 0.25f));
	
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Vampire::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (bTransformation)
		transformationProcess();
	else if (bHumanAspect)
	{
		humanAspectTime += deltaTime;

		if (humanAspectTime < 12000)
		{
			checkMovingBehavior();
			humanBehavior(deltaTime);
		}
		else
		{
			bTransformation = true;
			sprite->changeAnimation(TRANSFORM);
		}
	}
	else
		batBehavior(deltaTime);
}

// Private functions

void Vampire::humanBehavior(int deltaTime)
{
	posEnemy.y += FALL_STEP;

	if (bMoveRight)
	{
		bool bFloorDown = map->collisionMoveDown(posEnemy + glm::ivec2(24, 0), glm::ivec2(32, 32), &posEnemy.y);
		bool bShouldMoveLeft = map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), false) || !bFloorDown;
		 
		if (bShouldMoveLeft)
		{
			bMoveRight = false;
			sprite->changeAnimation(STAND_RIGHT);

			if (!bFloorDown)
				posEnemy.y -= FALL_STEP; // instead of falling, recover original y
		}
		else
			posEnemy.x += 1;
	}
	else
	{
		bool bFloorDown = map->collisionMoveDown(posEnemy - glm::ivec2(24, 0), glm::ivec2(32, 32), &posEnemy.y);
		bool bShouldMoveRight = map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), false) || !bFloorDown;

		if (bShouldMoveRight)
		{
			//posEnemy.x += 1;
			bMoveRight = true;
			sprite->changeAnimation(STAND_LEFT);

			if (!bFloorDown)
				posEnemy.y -= FALL_STEP; // instead of falling, recover original y
		}
		else
			posEnemy.x -= 1;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Vampire::batBehavior(int deltaTime)
{	
	bool bShouldMoveRight, bShouldMoveLeft, bShouldMoveUp, bShouldMoveDown;
	
	switch (batMov)
	{
	case RightUp:
		posEnemy.x += 1;
		posEnemy.y -= 1;

		bShouldMoveDown = map->collisionMoveUp(posEnemy, glm::ivec2(32, 32), &posEnemy.y, false);
		bShouldMoveLeft = map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), false);

		if (bShouldMoveDown && !map->collisionMoveRight(posEnemy + glm::ivec2(1, 1), glm::ivec2(32, 32), false))
		{
			batMov = RightDown;
		}
		else if (bShouldMoveLeft && !map->collisionMoveUp(posEnemy + glm::ivec2(-1, -1), glm::ivec2(32, 32), &posEnemy.y - 1, false))
		{
			batMov = LeftUp;
		}
		break;
	
	case LeftUp:
		posEnemy.x -= 1;
		posEnemy.y -= 1;

		bShouldMoveDown = map->collisionMoveUp(posEnemy, glm::ivec2(32, 32), &posEnemy.y, false);
		bShouldMoveRight = map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), false);

		if (bShouldMoveDown && !map->collisionMoveLeft(posEnemy + glm::ivec2(-1, +1), glm::ivec2(32, 32), false))
		{
			batMov = LeftDown;
		}
		else if (bShouldMoveRight && !map->collisionMoveUp(posEnemy + glm::ivec2(1, -1), glm::ivec2(32, 32), &posEnemy.y - 1, false))
		{
			batMov = RightUp;
		}
		break;

	case RightDown:
		posEnemy.x += 1;
		posEnemy.y += 1;

		bShouldMoveUp = map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y);
		bShouldMoveLeft = map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), false);

		if (bShouldMoveUp && !map->collisionMoveRight(posEnemy + glm::ivec2(1, -1), glm::ivec2(32, 32), false))
		{
			batMov = RightUp;
			if (abs(posEnemy.y - heightTransformation) >= 32)
			{
				bTransformation = true;
				bMoveRight = true;
				sprite->changeAnimation(UNTRANSFORM);
			}
		}
		else if (bShouldMoveLeft && !map->collisionMoveDown(posEnemy + glm::ivec2(-1, 1), glm::ivec2(32, 32), &posEnemy.y + 1))
			batMov = LeftDown;
		break;

	case LeftDown:
		posEnemy.x -= 1;
		posEnemy.y += 1;

		bShouldMoveUp = map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y);
		bShouldMoveRight = map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), false);

		if (bShouldMoveUp && !map->collisionMoveLeft(posEnemy + glm::ivec2(-1, -1), glm::ivec2(32, 32), false))
		{
			batMov = LeftUp;
			if (abs(posEnemy.y - heightTransformation) >= 32)
			{
				bTransformation = true;
				bMoveRight = false;
				sprite->changeAnimation(UNTRANSFORM);
			}
		}
		else if (bShouldMoveRight && !map->collisionMoveDown(posEnemy + glm::ivec2(1, 1), glm::ivec2(32, 32), &posEnemy.y + 1))
			batMov = RightDown;
		break;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Vampire::transformationProcess()
{
	if (sprite->getCurrentKeyFrame() == 5)
	{
		bHumanAspect = !bHumanAspect;

		if (bHumanAspect)
		{
			humanAspectTime = 0;
			if (bMoveRight)
				sprite->changeAnimation(MOVE_RIGHT);
			else
				sprite->changeAnimation(MOVE_LEFT);
		}
		else
		{
			heightTransformation = posEnemy.y;
			if (bMoveRight)
				batMov = RightUp;
			else
				batMov = LeftUp;

			sprite->changeAnimation(FLY);
		}
		bTransformation = false;
	}
}

void Vampire::checkMovingBehavior()
{
	if (bMoveRight)
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
	}
	else
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
	}

}