#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneInterface.h"
#include "SoundFactory.h"


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
	initPauseSprite(shaderProgram);
	initLifeUpSprite(shaderProgram);
	
	lives = 3;
	score = 0;
	remainingTime = 60;
	stageNumber = 1;
	char2id = { {'a',0}, {'c',1}, {'d',2}, {'e',3},
				{'g',4}, {'l',5}, {'m',6}, {'o',7},
				{'r',8}, {'s',9}, {'t',10}, {'v',11},
				{'y',12}, {'?',13}, {'x',14} };
	state = Starting;
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
	if (remainingTime < 15) {
		if (msTime % 200 <= 50) {
			renderNumber(remainingTime / 10, 238, 18);
			renderNumber(remainingTime % 10, 254, 18);
		}
		else if (msTime % 200 <= 100){
			renderNumber(remainingTime / 10, 243, 15);
			renderNumber(remainingTime % 10, 259, 15);
		}
		else if (msTime % 200 <= 150) {
			renderNumber(remainingTime / 10, 242, 19);
			renderNumber(remainingTime % 10, 258, 19);
		}
		else {
			renderNumber(remainingTime / 10, 239, 14);
			renderNumber(remainingTime % 10, 255, 14);
		}
	}
	else {
		renderNumber(remainingTime / 10, 240, 16);
		renderNumber(remainingTime % 10, 256, 16);
	}

	// stage
	renderCharacter('s', 368, 16);
	renderCharacter('t', 384, 16);
	renderCharacter('a', 400, 16);
	renderCharacter('g', 416, 16);
	renderCharacter('e', 432, 16);
	renderNumber(stageNumber / 10, 464, 16);
	renderNumber(stageNumber % 10, 480, 16);

	if (bLifeUp)
	{
		if (!bSoundLifeUpPlaying)
		{
			SoundFactory::instance().playLifeUp();
			bSoundLifeUpPlaying = true;
		} 
		if (!SoundFactory::instance().getLifeUpSoundFinished())
		{
			lifeUpSprite->setPosition(glm::vec2(posPlayer.x+16, posPlayer.y+32));
			lifeUpSprite->render();
		}
		else
		{
			bLifeUp = false;
			bSoundLifeUpPlaying = false;
		}
	}

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
	charactersSpritesheet.loadFromFile("images/chars.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

void SceneInterface::initPauseSprite(ShaderProgram& shaderProgram)
{
	pauseSpriteSheet.loadFromFile("images/pause.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pauseSpriteSheet.setMagFilter(GL_NEAREST);
	pauseSprite = Sprite::createSprite(glm::ivec2(200, 108), glm::vec2(1, 1), &pauseSpriteSheet, &shaderProgram);

	pauseSprite->setNumberAnimations(1);

	pauseSprite->setAnimationSpeed(0, 1);
	pauseSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	pauseSprite->changeAnimation(0);
	pauseSprite->setPosition(glm::vec2(156, 146));
}

void SceneInterface::initLifeUpSprite(ShaderProgram& shaderProgram)
{
	lifeUpSpriteSheet.loadFromFile("images/life_up.png", TEXTURE_PIXEL_FORMAT_RGBA);
	lifeUpSpriteSheet.setMagFilter(GL_NEAREST);
	lifeUpSprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(1, 1), &lifeUpSpriteSheet, &shaderProgram);

	lifeUpSprite->setNumberAnimations(1);

	lifeUpSprite->setAnimationSpeed(0, 1);
	lifeUpSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	lifeUpSprite->changeAnimation(0);
}

void SceneInterface::renderMessages()
{
	if (state == Starting)
	{
		renderCharacter('r', 208, 192);
		renderCharacter('e', 224, 192);
		renderCharacter('a', 240, 192);
		renderCharacter('d', 256, 192);
		renderCharacter('y', 272, 192);
		renderCharacter('?', 288, 192);
	}

	else if (state == Pause)
	{
		pauseSprite->render();
	}
	else if (state == StageCleared)
	{
		renderCharacter('s', 168, 192);
		renderCharacter('t', 184, 192);
		renderCharacter('a', 200, 192);
		renderCharacter('g', 216, 192);
		renderCharacter('e', 232, 192);

		renderCharacter('c', 264, 192);
		renderCharacter('l', 280, 192);
		renderCharacter('e', 296, 192);
		renderCharacter('a', 312, 192);
		renderCharacter('r', 328, 192);
	}
	else if (state == GameOver)
	{
		renderCharacter('g', 184, 192);
		renderCharacter('a', 200, 192);
		renderCharacter('m', 216, 192);
		renderCharacter('e', 232, 192);

		renderCharacter('o', 264, 192);
		renderCharacter('v', 280, 192);
		renderCharacter('e', 296, 192);
		renderCharacter('r', 312, 192);
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