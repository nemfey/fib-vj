#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Key.h"
#include "Game.h"

enum KeyAnims
{
	IDLE
};

// Public functions

void Key::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/key_static.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	
	//Animation
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(IDLE, 8);

	// Add frames to animation 0
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.5f));

	// Change to animation IDLE
	sprite->changeAnimation(IDLE);

	itemSize = glm::ivec2(8, 8);
}

void Key::update(int deltaTime)
{
	if (showing && collisionPlayer())
	{
		taken = true;
		showing = false;

		map->setKeyTaken(true);
	}
	
	sprite->update(deltaTime);
}

void Key::render()
{
	if (showing && !bPositioned)
	{
		glm::ivec2 screenCoords = map->getScreenCoords();
		if (!bPositioned)
		{
			posItem = map->getRandomPosition(7) - glm::vec2(0, 24);
			sprite->setPosition(glm::vec2(screenCoords.x + posItem.x, screenCoords.y + posItem.y));
			cout << "position assinged is: " << posItem.x << " " << posItem.y - 24 << endl;
			bPositioned = true;
		}
		//Key movement
		if (sprite->getCurrentKeyFrame() % 4 == 0) {
			sprite->setPosition(glm::vec2(screenCoords.x + posItem.x, screenCoords.y + posItem.y - 1));
		}
		else if (sprite->getCurrentKeyFrame() % 4 == 1 || sprite->getCurrentKeyFrame() % 4 == 3) {
			sprite->setPosition(glm::vec2(screenCoords.x + posItem.x, screenCoords.y + posItem.y));
		}
		else if (sprite->getCurrentKeyFrame() % 4 == 2) {
			sprite->setPosition(glm::vec2(screenCoords.x + posItem.x, screenCoords.y + posItem.y + 1));
		}
	}
	
	showing = map->allPlattformsStepped() && !taken;
	if (showing)
		sprite->render();
}
