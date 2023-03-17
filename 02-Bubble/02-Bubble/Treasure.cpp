#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Treasure.h"
#include "Game.h"

enum Treasures
{
	GEM1, GEM2, GEM3, COINS, FRUIT1, FRUIT2, FRUIT3, STAR
};

// Public functions

void Treasure::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);

	//Animation
	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(GEM1, 8);
	sprite->addKeyframe(GEM1, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(GEM2, 8);
	sprite->addKeyframe(GEM2, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(GEM3, 8);
	sprite->addKeyframe(GEM3, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(COINS, 8);
	sprite->addKeyframe(COINS, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(FRUIT1, 8);
	sprite->addKeyframe(FRUIT1, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(FRUIT2, 8);
	sprite->addKeyframe(FRUIT2, glm::vec2(0.25f, 0.25f));

	sprite->setAnimationSpeed(FRUIT3, 8);
	sprite->addKeyframe(FRUIT3, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(STAR, 8);
	sprite->addKeyframe(STAR, glm::vec2(0.75f, 0.f));

	//Set type to random between 0 and 7
	srand(time(NULL));
	type = rand() % 8;

	//DEBUG
	cout << "Setted treasure type to " << type << endl;

	// Change to animation the random type
	sprite->changeAnimation(type);

	//bIsTreasure = true;
	//bIsPickup = true;

	itemSize = glm::ivec2(12, 12);
}

void Treasure::update(int deltaTime)
{
	if (showing && collisionPlayer())
	{
		taken = true;
		showing = false;

		//DEBUG
		cout << "Treasure taken" << endl;
	}

	sprite->update(deltaTime);
}

void Treasure::render()
{
	if (!showing) {

		//Set type to random between 0 and 7
		srand(time(NULL));
		type = rand() % 8;

		//DEBUG
		cout << "Setted treasure type to " << type << endl;

		// Change to animation the random type
		sprite->changeAnimation(type);

		// Depending of the value it spawns further
		posItem = map->getRandomPosition(7 + type) - glm::vec2(0, 24);
		glm::ivec2 screenCoords = map->getScreenCoords();
		sprite->setPosition(glm::vec2(screenCoords.x + posItem.x, screenCoords.y + posItem.y));

		//DEBUG
		cout << "position assinged is: " << posItem.x << " " << posItem.y - 24 << endl;

		showing = true;
	}

	sprite->render();

}

int Treasure::getType() {
	return type;
}