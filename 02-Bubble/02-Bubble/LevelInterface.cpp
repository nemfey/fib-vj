#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "LevelInterface.h"


//#define SCREEN_X 144
// 003450

LevelInterface::LevelInterface()
{
	//map = NULL;

}

LevelInterface::~LevelInterface()
{
	//if (map != NULL)
	//	delete map;
}

// Public functions

void LevelInterface::init(ShaderProgram& shaderProgram)
{
	numbersSpritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numbersSpritesheet.setMagFilter(GL_NEAREST);

	remainingTimeSprites.push_back(Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1), &numbersSpritesheet, &shaderProgram));
	remainingTimeSprites.push_back(Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 1), &numbersSpritesheet, &shaderProgram));

	lives = 3;
	score = 0;
	remainingTime = 60;
	stage = 1;
}

void LevelInterface::addOneLive()
{
	lives++;
}
void LevelInterface::minusOneLive()
{
	lives--;
}

void LevelInterface::addScore(int s)
{
	score += s;
}

void LevelInterface::updateRemainingTime(int t)
{
	// cada update remaining time le sumaremos 0.1 a la textura del sprite en cuestion para avanzar en numero
	// esto solo funciona si cuando noos pasamos del `sprite volvemos al principio
	remainingTime = t;
}

void LevelInterface::updateStage(int s)
{
	stage = s;
}

void LevelInterface::render()
{
	glm::mat4 modelview;

	// rendering
}