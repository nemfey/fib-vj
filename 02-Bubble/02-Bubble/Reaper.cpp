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

enum ProjectileAnims
{
	LEFT, RIGHT
};

// Public functions

void Reaper::render()
{
	sprite->render();

	if (renderProjectile)
		projectile->render();
}

void Reaper::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	//Enemy
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

	sprite->setAnimationSpeed(ATTACK_LEFT, 12);
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

	sprite->setAnimationSpeed(ATTACK_RIGHT, 12);
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

	//Projectile
	projectileTex.loadFromFile("images/projectile.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projectileTex.setMagFilter(GL_NEAREST);
	projectile = Sprite::createSprite(glm::ivec2(80, 32), glm::vec2(0.5, 0.25), &projectileTex, &shaderProgram);
	projectile->setNumberAnimations(2);

	projectile->setAnimationSpeed(LEFT, 12);
	projectile->addKeyframe(LEFT, glm::vec2(0.f, 0.f));
	projectile->addKeyframe(LEFT, glm::vec2(0.5f, 0.f));
	projectile->addKeyframe(LEFT, glm::vec2(0.f, 0.25f));
	projectile->addKeyframe(LEFT, glm::vec2(0.5f, 0.25f));

	projectile->setAnimationSpeed(RIGHT, 12);
	projectile->addKeyframe(RIGHT, glm::vec2(0.f, 0.5f));
	projectile->addKeyframe(RIGHT, glm::vec2(0.5f, 0.5f));
	projectile->addKeyframe(RIGHT, glm::vec2(0.f, 0.75f));
	projectile->addKeyframe(RIGHT, glm::vec2(0.5f, 0.75f));
}

void Reaper::update(int deltaTime)
{

	sprite->update(deltaTime);
	projectile->update(deltaTime);

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

			bool bFloorDown = map->collisionMoveDown(posEnemy + glm::ivec2(24, 0), glm::ivec2(32, 32), &posEnemy.y, false);
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

			bool bFloorDown = map->collisionMoveDown(posEnemy - glm::ivec2(24, 0), glm::ivec2(32, 32), &posEnemy.y, false);
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

			if (sprite->getCurrentKeyFrame() == 4 && !bChargeSoundPlayed) {
				engine->play2D("sounds/charge3.wav", false);
				bChargeSoundPlayed = true;
			}

			if (sprite->getCurrentKeyFrame() == 8) {
				projectile->changeAnimation(RIGHT);
			}

			if (sprite->getCurrentKeyFrame() == 9 && !bAttackSoundPlayed) {
				engine->play2D("sounds/reaper_attack.wav", false);
				bAttackSoundPlayed = true;
			}

			if (sprite->getCurrentKeyFrame() >= 9 && sprite->getCurrentKeyFrame() < 12) {
				projectile->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x + 16), float(tileMapDispl.y + posEnemy.y)));
				
				renderProjectile = true;
			}

			else if (sprite->getCurrentKeyFrame() == 12) {
				attackEnded = true;
				renderProjectile = false;

				bChargeSoundPlayed = false;
				bAttackSoundPlayed = false;
			}
		}

		else {

			if (sprite->animation() != ATTACK_LEFT)
				sprite->changeAnimation(ATTACK_LEFT);

			if (sprite->getCurrentKeyFrame() == 4 && !bChargeSoundPlayed) {
				engine->play2D("sounds/charge3.wav", false);
				bChargeSoundPlayed = true;
			}


			if (sprite->getCurrentKeyFrame() == 8) {
				projectile->changeAnimation(LEFT);
			}

			if (sprite->getCurrentKeyFrame() == 9 && !bAttackSoundPlayed) {
				engine->play2D("sounds/reaper_attack.wav", false);
				bAttackSoundPlayed = true;
			}

			if (sprite->getCurrentKeyFrame() >= 9 && sprite->getCurrentKeyFrame() < 12) {
				projectile->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x - 64), float(tileMapDispl.y + posEnemy.y)));

				renderProjectile = true;
			}

			if (sprite->getCurrentKeyFrame() == 12) {
				attackEnded = true;
				renderProjectile = false;

				bChargeSoundPlayed = false;
				bAttackSoundPlayed = false;
			}
		}

	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

bool Reaper::collisionPlayer()
{
	glm::ivec2 posPlayer = map->getPosPlayer();
	int tileSize = map->getTileSize();

	bool collisionX;
	bool collisionY;

	//Attacking left
	if (sprite->animation() == ATTACK_LEFT && sprite->getCurrentKeyFrame() >= 10 && sprite->getCurrentKeyFrame() < 12) {
		collisionX = posPlayer.x + 32 >= posEnemy.x - 52 && posEnemy.x + hitbox.x - 52 >= posPlayer.x;
		collisionY = posPlayer.y + 32 >= posEnemy.y + (32 - hitbox.y) && posEnemy.y + 32 - (32 - hitbox.y) >= posPlayer.y;
	}
	//Attacking right
	else if (sprite->animation() == ATTACK_RIGHT && sprite->getCurrentKeyFrame() >= 10 && sprite->getCurrentKeyFrame() < 12) {
		collisionX = posPlayer.x + 32 >= posEnemy.x + hitbox.x && posEnemy.x + hitbox.x + 52 >= posPlayer.x;
		collisionY = posPlayer.y + 32 >= posEnemy.y + (32 - hitbox.y) && posEnemy.y + 32 - (32 - hitbox.y) >= posPlayer.y;
	}
	else {
		collisionX = posPlayer.x + 32 >= posEnemy.x + (32 - hitbox.x) && posEnemy.x + 32 - (32 - hitbox.x) >= posPlayer.x;
		collisionY = posPlayer.y + 32 >= posEnemy.y + (32 - hitbox.y) && posEnemy.y + 32 - (32 - hitbox.y) >= posPlayer.y;
	}

	return collisionX && collisionY;
}