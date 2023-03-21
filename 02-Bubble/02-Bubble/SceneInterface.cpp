#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneInterface.h"


//#define SCREEN_X 144
// 003450

SceneInterface::SceneInterface()
{
	numberSprite = NULL;
	characterSprite = NULL;
	heartSprite = NULL;
}

SceneInterface::~SceneInterface()
{
	if (numberSprite != NULL)
		delete numberSprite;
	if (characterSprite != NULL)
		delete characterSprite;
	if (heartSprite != NULL)
		delete heartSprite;
}

// Public functions

void SceneInterface::init(ShaderProgram &shaderProgram)
{
	initNumberSprite(shaderProgram);
	initCharacterSprite(shaderProgram);
	initHeartSprite(shaderProgram);
	
	lives = 3;
	score = 0;
	remainingTime = 60;
	stageNumber = 1;
	//bStageClear = false;
	char2id = { {'a',0}, {'c',1}, {'d',2}, {'e',3},
				{'g',4}, {'l',5}, {'m',6}, {'o',7},
				{'r',8}, {'s',9}, {'t',10}, {'v',11},
				{'y',12}, {'?',13}, {'x',14} };
	state = Playing;
}

void SceneInterface::updateLives(int l)
{
	lives = l;
}

void SceneInterface::updateScore(int s)
{
	score = s;
}

void SceneInterface::updateRemainingTime(int t)
{
	remainingTime = t;
}

void SceneInterface::updateStageNumber(int s)
{
	stageNumber = s;
}

void SceneInterface::render()
{
	// lives
	heartSprite->changeAnimation(0);
	heartSprite->setPosition(glm::vec2(16, 16));
	heartSprite->render();
	renderCharacter('x', 32, 16);
	renderNumber(lives, 48, 16);

	// score
	renderNumber(score / 100000, 104, 16);
	renderNumber((score%100000) / 10000, 120, 16);
	renderNumber((score % 10000) / 1000, 136, 16);
	renderNumber((score % 1000) / 100, 152, 16);
	renderNumber((score % 100) / 10, 168, 16);
	renderNumber(score % 10, 184, 16);

	// time
	renderNumber(remainingTime / 10, 240, 16);
	renderNumber(remainingTime % 10, 256, 16);

	// stage
	renderCharacter('s', 368, 16);
	renderCharacter('t', 384, 16);
	renderCharacter('a', 400, 16);
	renderCharacter('g', 416, 16);
	renderCharacter('e', 432, 16);
	renderNumber(stageNumber / 10, 464, 16);
	renderNumber(stageNumber % 10, 480, 16);

	renderMessages();
}

// Private functions

void SceneInterface::initNumberSprite(ShaderProgram& shaderProgram)
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

void SceneInterface::initCharacterSprite(ShaderProgram& shaderProgram)
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

void SceneInterface::initHeartSprite(ShaderProgram& shaderProgram)
{
	heartSpritesheet.loadFromFile("images/heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	heartSpritesheet.setMagFilter(GL_NEAREST);
	heartSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1, 1), &heartSpritesheet, &shaderProgram);

	heartSprite->setNumberAnimations(1);

	heartSprite->setAnimationSpeed(0, 1);
	heartSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
}

void SceneInterface::renderMessages()
{
	//cout << state << endl;
	//cout << "hola" << endl;
	if (state == StageCleared)
	{
		renderCharacter('s', 160, 224);
		renderCharacter('t', 176, 224);
		renderCharacter('a', 192, 224);
		renderCharacter('g', 208, 224);
		renderCharacter('e', 224, 224);
		renderCharacter('c', 256, 224);
		renderCharacter('l', 272, 224);
		renderCharacter('e', 288, 224);
		renderCharacter('a', 304, 224);
		renderCharacter('r', 320, 224);
	}
	else if (state == GameOver)
	{
		renderCharacter('g', 160, 224);
		renderCharacter('a', 176, 224);
		renderCharacter('m', 192, 224);
		renderCharacter('e', 208, 224);
		//renderCharacter('e', 224, 224);
		//renderCharacter('c', 256, 224);
		renderCharacter('o', 272, 224);
		renderCharacter('v', 288, 224);
		renderCharacter('e', 304, 224);
		renderCharacter('r', 320, 224);
	}
}

void SceneInterface::renderNumber(int n, int x, int y)
{
	numberSprite->changeAnimation(n);
	numberSprite->setPosition(glm::vec2(x, y));
	numberSprite->render();
}

void SceneInterface::renderCharacter(char c, int x, int y)
{
	characterSprite->changeAnimation(char2id[c]);
	characterSprite->setPosition(glm::vec2(x, y));
	characterSprite->render();
}