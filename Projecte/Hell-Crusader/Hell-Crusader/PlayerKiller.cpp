#include <GL/glew.h>
#include <GL/glut.h>
#include "PlayerKiller.h"
#include "Game.h"

#define FALL_STEP 4

enum playerKillers
{
	LAVA, SPIKES, SAW, POISON, MUSHROOMS
};

// Public functions

PlayerKiller::PlayerKiller(int id)
{
	type = id;
}

void PlayerKiller::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	isPlayerKiller = true;

	spritesheet.loadFromFile("images/playerkiller2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(LAVA, 6);
	sprite->addKeyframe(LAVA, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(LAVA, glm::vec2(0.125f, 0.f));
	sprite->addKeyframe(LAVA, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(LAVA, glm::vec2(0.375f, 0.f));

	sprite->setAnimationSpeed(SAW, 10);
	sprite->addKeyframe(SAW, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(SAW, glm::vec2(0.625f, 0.f));
	sprite->addKeyframe(SAW, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(SAW, glm::vec2(0.875f, 0.f));

	sprite->setAnimationSpeed(POISON, 8);
	sprite->addKeyframe(POISON, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(POISON, glm::vec2(0.125f, 0.25f));
	sprite->addKeyframe(POISON, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(POISON, glm::vec2(0.375f, 0.25f));

	sprite->setAnimationSpeed(SPIKES, 8);
	sprite->addKeyframe(SPIKES, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(SPIKES, glm::vec2(0.625f, 0.25f));
	sprite->addKeyframe(SPIKES, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(SPIKES, glm::vec2(0.875f, 0.25f));

	sprite->setAnimationSpeed(MUSHROOMS, 8);
	sprite->addKeyframe(MUSHROOMS, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MUSHROOMS, glm::vec2(0.125f, 0.5f));
	sprite->addKeyframe(MUSHROOMS, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(MUSHROOMS, glm::vec2(0.375f, 0.5f));

	sprite->changeAnimation(type);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

	hitbox = glm::ivec2(16,16);
}

void PlayerKiller::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void PlayerKiller::setType(int id)
{
	type = id;
}

int PlayerKiller::getType()
{
	return type;
}

bool PlayerKiller::collisionPlayer()
{
	glm::ivec2 posPlayer = map->getPosPlayer();
	int tileSize = map->getTileSize();

	bool collisionX;
	bool collisionY;

	//LAVA
	if (type == LAVA) {
		collisionX = posPlayer.x + 32 >= posEnemy.x && posEnemy.x + hitbox.x >= posPlayer.x;
		collisionY = posPlayer.y + 32 >= posEnemy.y + 6 && posEnemy.y + 6 >= posPlayer.y;
	}
	//SAW
	else if (type == SAW) {
		collisionX = posPlayer.x + 32 >= posEnemy.x + 2 && posEnemy.x + hitbox.x - 2 >= posPlayer.x;
		collisionY = posPlayer.y + 32 >= posEnemy.y + 2 && posEnemy.y + hitbox.y - 2 >= posPlayer.y;
	}
	else {
		collisionX = posPlayer.x + 32 >= posEnemy.x + (32 - hitbox.x) && posEnemy.x + 32 - (32 - hitbox.x) >= posPlayer.x;
		collisionY = posPlayer.y + 32 >= posEnemy.y + (32 - hitbox.y) && posEnemy.y + 32 - (32 - hitbox.y) >= posPlayer.y;
	}

	return collisionX && collisionY;
}