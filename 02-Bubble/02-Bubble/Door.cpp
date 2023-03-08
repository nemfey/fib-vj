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
	spritesheet.loadFromFile("images/hell-door-2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	//Animations
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(CLOSED, 1);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(OPEN, 1);
	sprite->addKeyframe(OPEN, glm::vec2(0.75f, 0.5f));

	//Set animation
	sprite->setAnimationSpeed(OPENING, 5);
	sprite->addKeyframe(OPENING, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(0.5f, 0.5f));

	posItem = map->getDoorPosition() * 16;
	//sprite->setPosition(posItem + glm::ivec2(32, 16));
	glm::ivec2 screenCoords = map->getScreenCoords();
	sprite->setPosition(posItem + screenCoords);

	itemSize = glm::ivec2(32, 32);

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
		//sprite->changeAnimation(OPEN);
		//showing = true;
	}
	else if (showing && collisionPlayer())
	{
		cout << "LEVEL FINISHED" << endl;
	}
}

void Door::render()
{
	sprite->render();
}

// Private functions

void Door::openingProcess()
{
	cout << "operningdorrororo" << endl;
	if (sprite->getCurrentKeyFrame() == 5)
	{
		sprite->changeAnimation(OPEN);
		showing = true;
		bOpening = false;
	}
}