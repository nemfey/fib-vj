#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Reaper.h"

#define FALL_STEP 4

enum ReaperAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, ATTACK_LEFT, ATTACK_RIGHT
};

// Public functions

void Reaper::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/reaper.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.375f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.875f, 0.0f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.625f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.875f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.625f, 0.f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 10);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.f, 0.25));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.125f, 0.25));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.25f, 0.25));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.375f, 0.25));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.5f, 0.25));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.625f, 0.25));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.75f, 0.25));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.875f, 0.25));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.f, 0.5));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.125f, 0.5));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.25f, 0.5));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.375f, 0.5));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(0.f, 0.25));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 10);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.5f, 0.5));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.625f, 0.5));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.75f, 0.5));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.875f, 0.5));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.f, 0.75));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.125f, 0.75));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.25f, 0.75));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.375f, 0.75));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.5f, 0.75));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.625f, 0.75));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.75f, 0.75));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.875f, 0.75));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.5f, 0.5));

	sprite->changeAnimation(STAND_LEFT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

	hitbox = glm::ivec2(24, 24);
}

void Reaper::update(int deltaTime)
{

	sprite->update(deltaTime);

	//Check if the player is nearby
	if (((sprite->animation() == MOVE_LEFT && (map->getPosPlayer()).x + 32 > posEnemy.x - 2 * 16 && (map->getPosPlayer()).x < posEnemy.x)
		|| (sprite->animation() == MOVE_RIGHT && (map->getPosPlayer()).x > posEnemy.x + 32 && (map->getPosPlayer()).x < posEnemy.x + 32 + 2 * 16))
		&& ((map->getPosPlayer()).y >= posEnemy.y - 16 && (map->getPosPlayer()).y <= posEnemy.y + 16) && attackEnded) {
		attacking = true; attackEnded = false;
	}
	else {
		attacking = false;
	}

	if (!attacking && attackEnded) {
		if (moveRight)
		{
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);

			//posEnemy.x += 1;
			posEnemy.y += FALL_STEP;

			bool bFloorDown = map->collisionMoveDown(posEnemy + glm::ivec2(24, 0), glm::ivec2(32, 32), &posEnemy.y);
			bool bShouldMoveLeft = map->collisionMoveRight(posEnemy, glm::ivec2(32, 32), false) || !bFloorDown;

			if (bShouldMoveLeft)
			{
				//posEnemy.x -= 1;
				sprite->changeAnimation(STAND_RIGHT);
				moveRight = false;

				if (!bFloorDown)
					posEnemy.y -= FALL_STEP; // instead of falling, recover original y
			}
			else
				posEnemy.x += 1;
		}
		else
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);

			//posEnemy.x -= 1;
			posEnemy.y += FALL_STEP;

			bool bFloorDown = map->collisionMoveDown(posEnemy - glm::ivec2(24, 0), glm::ivec2(32, 32), &posEnemy.y);
			bool bShouldMoveRight = map->collisionMoveLeft(posEnemy, glm::ivec2(32, 32), false) || !bFloorDown;

			if (bShouldMoveRight)
			{
				//posEnemy.x += 1;
				sprite->changeAnimation(STAND_LEFT);
				moveRight = true;

				if (!bFloorDown)
					posEnemy.y -= FALL_STEP; // instead of falling, recover original y
			}
			else
				posEnemy.x -= 1;
		}
	}
	else {

		if (moveRight) {

			if (sprite->animation() != ATTACK_RIGHT)
				sprite->changeAnimation(ATTACK_RIGHT);

			if (sprite->getCurrentKeyFrame() == 12) {
				attackEnded = true;
			}
		}

		else {

			if (sprite->animation() != ATTACK_LEFT)
				sprite->changeAnimation(ATTACK_LEFT);

			if (sprite->getCurrentKeyFrame() == 12) {
				attackEnded = true;
			}
		}

	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}