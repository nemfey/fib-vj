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
	// lives
	heartSprite->changeAnimation(0);
	heartSprite->setPosition(glm::vec2(16, 16));
	heartSprite->render();
	// DIBUJAR LA X
	numberSprite->changeAnimation(lives);
	numberSprite->setPosition(glm::vec2(48, 16));
	numberSprite->render();

	// time
	numberSprite->changeAnimation(remainingTime / 10);
	numberSprite->setPosition(glm::vec2(240, 16));
	numberSprite->render();
	numberSprite->changeAnimation(remainingTime % 10);
	numberSprite->setPosition(glm::vec2(256, 16));
	numberSprite->render();

	// rendering
}

// Private functions

void LevelInterface::initNumberSprite(ShaderProgram& shaderProgram)
{
	numbersSpritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numbersSpritesheet.setMagFilter(GL_NEAREST);
	numberSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 1), &numbersSpritesheet, &shaderProgram);
	
	numberSprite->setNumberAnimations(10);

	numberSprite->setAnimationSpeed(0, 1);
	numberSprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	numberSprite->setAnimationSpeed(1, 1);
	numberSprite->addKeyframe(1, glm::vec2(0.1f, 0.f));

	numberSprite->setAnimationSpeed(2, 1);
	numberSprite->addKeyframe(2, glm::vec2(0.2f, 0.f));

	numberSprite->setAnimationSpeed(3, 1);
	numberSprite->addKeyframe(3, glm::vec2(0.3f, 0.f));

	numberSprite->setAnimationSpeed(4, 1);
	numberSprite->addKeyframe(4, glm::vec2(0.4f, 0.f));

	numberSprite->setAnimationSpeed(5, 1);
	numberSprite->addKeyframe(5, glm::vec2(0.5f, 0.f));

	numberSprite->setAnimationSpeed(6, 1);
	numberSprite->addKeyframe(6, glm::vec2(0.6f, 0.f));

	numberSprite->setAnimationSpeed(7, 1);
	numberSprite->addKeyframe(7, glm::vec2(0.7f, 0.f));

	numberSprite->setAnimationSpeed(8, 1);
	numberSprite->addKeyframe(8, glm::vec2(0.8f, 0.f));

	numberSprite->setAnimationSpeed(9, 1);
	numberSprite->addKeyframe(9, glm::vec2(0.9f, 0.f));
}

void LevelInterface::initCharacterSprite(ShaderProgram& shaderProgram)
{
	charactersSpritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	charactersSpritesheet.setMagFilter(GL_NEAREST);
	characterSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 1), &charactersSpritesheet, &shaderProgram);
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