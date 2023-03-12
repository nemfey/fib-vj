#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "LevelInterface.h"


//#define SCREEN_X 144
// 003450

LevelInterface::LevelInterface()
{
	numberSprite = NULL;
	characterSprite = NULL;
	heartSprite = NULL;
}

LevelInterface::~LevelInterface()
{
	if (numberSprite != NULL)
		delete numberSprite;
	if (characterSprite != NULL)
		delete characterSprite;
	if (heartSprite != NULL)
		delete heartSprite;
}

// Public functions

void LevelInterface::init(ShaderProgram& shaderProgram)
{
	initNumberSprite(shaderProgram);
	initCharacterSprite(shaderProgram);
	initHeartSprite(shaderProgram);
	
	lives = 3;
	score = 0;
	remainingTime = 60;
	stage = 1;
	char2id = { {'a',0}, {'c',1}, {'d',2}, {'e',3},
				{'g',4}, {'l',5}, {'m',6}, {'o',7},
				{'r',8}, {'s',9}, {'t',10}, {'v',11},
				{'y',12}, {'?',13}, {'x',14} };
}

void LevelInterface::updateLives(int l)
{
	lives = l;
}

void LevelInterface::addScore(int s)
{
	score += s;
}

void LevelInterface::updateRemainingTime(int t)
{
	remainingTime = t;
}

void LevelInterface::updateStage(int s)
{
	stage = s;
}

void LevelInterface::render()
{
	// lives
	heartSprite->changeAnimation(0);
	heartSprite->setPosition(glm::vec2(16, 16));
	heartSprite->render();
	renderCharacter('x', 32, 16);
	renderNumber(lives, 48, 16);

	// score
	//renderNumber(score / 10, 240, 16);
	//renderNumber(score % 10, 256, 16);

	// time
	renderNumber(remainingTime / 10, 240, 16);
	renderNumber(remainingTime % 10, 256, 16);

	// stage
	renderCharacter('s', 384, 16);
	renderCharacter('t', 400, 16);
	renderCharacter('a', 416, 16);
	renderCharacter('g', 432, 16);
	renderCharacter('e', 448, 16);
	renderNumber(stage / 10, 480, 16);
	renderNumber(stage % 10, 496, 16);
}

// Private functions

void LevelInterface::initNumberSprite(ShaderProgram& shaderProgram)
{
	numbersSpritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numbersSpritesheet.setMagFilter(GL_NEAREST);
	numberSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 1), &numbersSpritesheet, &shaderProgram);
	
	numberSprite->setNumberAnimations(10);

	for (int i = 0; i < 10; ++i)
	{
		numberSprite->setAnimationSpeed(i, 1);
		numberSprite->addKeyframe(i, glm::vec2(i/10.f, 0.f));
	}
}

void LevelInterface::initCharacterSprite(ShaderProgram& shaderProgram)
{
	charactersSpritesheet.loadFromFile("images/characters.png", TEXTURE_PIXEL_FORMAT_RGBA);
	charactersSpritesheet.setMagFilter(GL_NEAREST);
	characterSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.25), &charactersSpritesheet, &shaderProgram);

	characterSprite->setNumberAnimations(16);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4;++j)
		{
			int animId = i * 4 + j;
			characterSprite->setAnimationSpeed(animId, 1);
			characterSprite->addKeyframe(animId, glm::vec2(j/4.f, i/4.f));
		}
}

void LevelInterface::initHeartSprite(ShaderProgram& shaderProgram)
{
	heartSpritesheet.loadFromFile("images/heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	heartSpritesheet.setMagFilter(GL_NEAREST);
	heartSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1, 1), &heartSpritesheet, &shaderProgram);

	heartSprite->setNumberAnimations(1);

	heartSprite->setAnimationSpeed(0, 1);
	heartSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
}

void LevelInterface::renderNumber(int n, int x, int y)
{
	numberSprite->changeAnimation(n);
	numberSprite->setPosition(glm::vec2(x, y));
	numberSprite->render();
}

void LevelInterface::renderCharacter(char c, int x, int y)
{
	characterSprite->changeAnimation(char2id[c]);
	characterSprite->setPosition(glm::vec2(x, y));
	characterSprite->render();
}