#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"
#include "Game.h"

enum DoorAnims
{
	CLOSED, OPEN, OPENING
};

// Public functions

void Door::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	//spritesheet.loadFromFile("images/door_hell.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.loadFromFile("images/hell-door-5.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	//Animations
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(CLOSED, 1);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(OPEN, 1);
	sprite->addKeyframe(OPEN, glm::vec2(0.75f, 0.5f));

	//Set animation
	sprite->setAnimationSpeed(OPENING, 4);
	sprite->addKeyframe(OPENING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(0.75f, 0.5f));

	posItem = map->getDoorPosition() * 16;
	//sprite->setPosition(posItem + glm::ivec2(32, 16));
	glm::ivec2 screenCoords = map->getScreenCoords();
	sprite->setPosition(posItem + screenCoords);

	hitbox = glm::ivec2(2, 2);

	cout << "Door position: " << posItem.x << " " << posItem.y << endl;
}

void Door::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (bOpening)
	{
		openingProcess();
	}
	else if (!showing && map->getKeyTaken())
	{
		sprite->changeAnimation(OPENING);
		bOpening = true;
	}
	else if (showing && collisionPlayer())
	{
		taken = true;
		cout << "LEVEL FINISHED" << endl;
	}
}

void Door::render()
{
	if (showing || bOpening)
		sprite->render();
}

// Private functions

void Door::openingProcess()
{
	if (sprite->getCurrentKeyFrame() == 7)
	{
		sprite->changeAnimation(OPEN);
		showing = true;
		bOpening = false;
	}
}