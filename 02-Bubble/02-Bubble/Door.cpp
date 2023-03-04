#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"
#include "Game.h"

enum DoorAnims
{
	CLOSED, OPEN
};


void Door::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/door.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 1), &spritesheet, &shaderProgram);

	//Animations
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(CLOSED, 1);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(OPEN, 1);
	sprite->addKeyframe(OPEN, glm::vec2(0.5f, 0.f));

	//Set animation
	sprite->changeAnimation(CLOSED);

	posItem = map->getDoorPosition() * 16;
	//sprite->setPosition(posItem + glm::ivec2(32, 16));
	glm::ivec2 screenCoords = map->getScreenCoords();
	sprite->setPosition(posItem + screenCoords);

	// key set position
	//sprite->setPosition(glm::vec2(pos.x * 16 + 32, pos.y * 16 - 8));

	//DEBUG
	itemSize = glm::ivec2(32, 32);

	cout << "Door position: " << posItem.x << " " << posItem.y << endl;
}

void Door::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (!showing && map->getKeyTaken())
	{
		sprite->changeAnimation(OPEN);
		showing = true;
	}
	else if (showing && collisionPlayer())
	{
		cout << "LEVEL FINISHED" << endl;
	}
	//else sprite->changeAnimation(CLOSED);
}

void Door::render()
{
	sprite->render();
}