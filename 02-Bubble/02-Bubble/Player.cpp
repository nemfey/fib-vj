#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 56
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, DEATH, INVINCIBLE, SPAWN
};

Player::~Player()
{
	if (sprite != NULL)
		delete sprite;
	if (map != NULL)
		delete map;
}

// Public functions

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.125f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.625f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 12);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.875f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.875f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 12);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.625, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.875, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.625, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.25f));

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.125f, 0.5f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.375f, 0.5f));

	sprite->setAnimationSpeed(DEATH, 8);
	sprite->addKeyframe(DEATH, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DEATH, glm::vec2(0.625f, 0.5f));
	sprite->addKeyframe(DEATH, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(DEATH, glm::vec2(0.875f, 0.5f));
	sprite->addKeyframe(DEATH, glm::vec2(0.f, 0.75f));

	sprite->setAnimationSpeed(INVINCIBLE, 8);
	sprite->addKeyframe(INVINCIBLE, glm::vec2(0.125f, 0.75f));
	sprite->addKeyframe(INVINCIBLE, glm::vec2(0.25f, 0.75f));
	sprite->addKeyframe(INVINCIBLE, glm::vec2(0.375f, 0.75f));
	sprite->addKeyframe(INVINCIBLE, glm::vec2(0.25f, 0.75f));

	sprite->setAnimationSpeed(SPAWN, 12);
	sprite->addKeyframe(SPAWN, glm::vec2(0.5f, 0.75f));
	sprite->addKeyframe(SPAWN, glm::vec2(0.625f, 0.75f));
	sprite->addKeyframe(SPAWN, glm::vec2(0.75f, 0.75f));
	sprite->addKeyframe(SPAWN, glm::vec2(0.875f, 0.75f));
	sprite->addKeyframe(SPAWN, glm::vec2(0.875f, 0.75f));

	sprite->changeAnimation(SPAWN);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (bSpawning) {
		if (sprite->animation() != SPAWN)
			sprite->changeAnimation(SPAWN);

		if (sprite->getCurrentKeyFrame() >= 4) {
			bSpawning = false;
		}
	}
	else if (bDying)
	{
		if (sprite->animation() != DEATH)
			sprite->changeAnimation(DEATH);

		if (sprite->getCurrentKeyFrame() >= 4) {
			bDying = false;
			bRespawn = true;
		}
	}
	else {

		if (bInvincible && sprite->animation() != INVINCIBLE) {
			sprite->changeAnimation(INVINCIBLE);
		}

		else if (inmunityState)
		{
			if (sprite->animation() != INVINCIBLE)
				sprite->changeAnimation(INVINCIBLE);

			inmunityTime -= deltaTime;
			if (inmunityTime < 0)
			{
				cout << "my inmunity state ended :(" << endl;
				inmunityState = false;
				inmunityTime = 0;
			}
		}


		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			bFacingRight = false;
			if (sprite->animation() != MOVE_LEFT && !bInvincible && !inmunityState && !bJumping)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), true))
			{
				posPlayer.x += 2;

				if (!bInvincible && !inmunityState)
					sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			bFacingRight = true;
			if (sprite->animation() != MOVE_RIGHT && !bInvincible && !inmunityState && !bJumping)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), true))
			{
				posPlayer.x -= 2;
				if (!bInvincible && !inmunityState)
					sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (!bFacingRight && sprite->animation() != STAND_LEFT && !bInvincible && !inmunityState && !bJumping)
				sprite->changeAnimation(STAND_LEFT);
			else if (bFacingRight && sprite->animation() != STAND_RIGHT && !bInvincible && !inmunityState && !bJumping)
				sprite->changeAnimation(STAND_RIGHT);
		}
		if (bJumping)
		{
			if (!bFacingRight && sprite->animation() != JUMP_LEFT && !bInvincible && !inmunityState)
				sprite->changeAnimation(JUMP_LEFT);
			else if (bFacingRight && sprite->animation() != JUMP_RIGHT && !bInvincible && !inmunityState)
				sprite->changeAnimation(JUMP_RIGHT);
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, true);
			}
			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y, true))
				bJumping = false;
		}
		else
		{
			/*
			if (!bFacingRight && sprite->animation() != STAND_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			if (bFacingRight && sprite->animation() != STAND_RIGHT && !bInvincible && !inmunityState)
				sprite->changeAnimation(STAND_RIGHT);
			*/

			posPlayer.y += FALL_STEP;
			// y no estamos dentro de pierda 5 o 6
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y, true))
			{
				posPlayer.y += FALL_STEP;
				map->positionStepped(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
	}
	//Invincible floating effect
	if (sprite->animation() == INVINCIBLE && sprite->getCurrentKeyFrame() % 4 == 0) {
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 1)));
	}
	else if (sprite->animation() == INVINCIBLE && sprite->getCurrentKeyFrame() % 4 == 2) {
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y + 1)));
	}
	else sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	if (!inmunityState)
		sprite->render();
	else if (!bInvincible) {
		if (inmunityTime < 1000) {
			if (inmunityTime % 125 <= 65)
				sprite->render();
		}
		else {
			if (inmunityTime % 250 <= 200)
				sprite->render();
		}
	}
	else sprite->render();
}

void Player::loseLive()
{
	bDying = true;
	if (lives > 0 && !inmunityState)
	{
		--lives;
		setImmune(3000);
		cout << "you lost a live but now im inmune" << endl;
		// AÑADIR INMUNIDAD
	}
	else if (lives == 0)
		cout << "GAME OVER" << endl;
}

void Player::resetPosition(const glm::ivec2& pos)
{
	bJumping = false;
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::addScore(int s)
{
	score += s;
}

// Getters & Setters

void Player::setPosition(const glm::ivec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setImmune(int time) {
	inmunityState = true;
	inmunityTime = time;
}