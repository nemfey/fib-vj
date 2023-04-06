#include <cmath>
#include <iostream>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Treasure.h"
#include "Game.h"

enum Treasures
{
	COINS, GEM1, GEM2, GEM3, FRUIT1, FRUIT2, FRUIT3, STAR
};

// Public functions

void Treasure::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/treasures.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);

	//Animation
	sprite->setNumberAnimations(8);

	//Set all frames (They are in order in the spriteSheet)
	for (int i = 0; i < 8; ++i) {
		sprite->setAnimationSpeed(i, 8);
		for (int j = 0; j < 8; ++j) {
			sprite->addKeyframe(i, glm::vec2(0.125*j, 0.125*i));
		}
	}

	//Set type to random between 0 and 7
	srand(time(NULL));
	type = rand() % 8;

	//DEBUG
	//cout << "Setted treasure type to " << type << endl;

	// Change to animation the random type
	sprite->changeAnimation(type);

	//bIsTreasure = true;
	//bIsPickup = true;

	itemSize = glm::ivec2(16, 16);
	hitbox = glm::ivec2(12, 12);
}

void Treasure::update(int deltaTime)
{
	if (showing && collisionPlayer())
	{
		taken = true;
		showing = false;

		//DEBUG
		//cout << "Treasure taken" << endl;
	}

	sprite->update(deltaTime);
}

void Treasure::render()
{
	glm::ivec2 screenCoords = map->getScreenCoords();
	if (!showing) {

		//Set type to random between 0 and 7
		resetType();

		//DEBUG
		//cout << "Setted treasure type to " << type << endl;

		// Change to animation the random type
		sprite->changeAnimation(type);

		// Depending of the value it spawns further
		posItem = map->getRandomPosition(7) - glm::vec2(0, 24);
		sprite->setPosition(glm::vec2(screenCoords.x + posItem.x, screenCoords.y + posItem.y));

		//DEBUG
		//cout << "position assinged is: " << posItem.x << " " << posItem.y - 24 << endl;

		showing = true;
	}

	//Movement
	if (sprite->getCurrentKeyFrame() % 4 == 0) {
		sprite->setPosition(glm::vec2(screenCoords.x + posItem.x, screenCoords.y + posItem.y - 1));
	}
	else if (sprite->getCurrentKeyFrame() % 4 == 1 || sprite->getCurrentKeyFrame() % 4 == 3) {
		sprite->setPosition(glm::vec2(screenCoords.x + posItem.x, screenCoords.y + posItem.y));
	}
	else if (sprite->getCurrentKeyFrame() % 4 == 2) {
		sprite->setPosition(glm::vec2(screenCoords.x + posItem.x, screenCoords.y + posItem.y + 1));
	}

	sprite->render();

}

int Treasure::getType() {
	return type;
}

void Treasure::resetType() {
	srand(time(NULL));
	type = rand() % 8;
}