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
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.25f, 0.75f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.75f, 0.75f));

	sprite->setAnimationSpeed(DEATH, 8);
	sprite->addKeyframe(DEATH, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(INVINCIBLE, 8);
	sprite->addKeyframe(INVINCIBLE, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(SPAWN, 8);
	sprite->addKeyframe(SPAWN, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(SPAWN);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32), bJumping))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32), bJumping))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	if (bJumping)
	{
		if ((sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) && sprite->animation() != JUMP_LEFT)
			sprite->changeAnimation(JUMP_LEFT);
		else if ((sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) && sprite->animation() != JUMP_RIGHT)
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
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
		if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y, true))
			bJumping = false;
	}
	else
	{

		if (sprite->animation() == JUMP_LEFT && sprite->animation() != STAND_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		if (sprite->animation() == JUMP_RIGHT && sprite->animation() != STAND_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);

		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
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

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	if (inmunityState)
	{
		if (sprite->animation() != INVINCIBLE)
			sprite->changeAnimation(INVINCIBLE);

		inmunityTime += deltaTime;
		if (inmunityTime > 3000)
		{
			cout << "my inmunity state ended :(" << endl;
			inmunityState = false;
			inmunityTime = 0;
		}
	}
}

void Player::render()
{
	if (!inmunityState)
		sprite->render();
	else
		if (inmunityTime % 250 <= 125)
			sprite->render();
}

void Player::loseLive()
{
	if (lives > 0 && !inmunityState)
	{
		--lives;
		inmunityState = true;
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