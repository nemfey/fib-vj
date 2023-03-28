#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Menu.h"


//#define SCREEN_X 144
//#define SCREEN_Y 99
#define SCREEN_X 0
#define SCREEN_Y 48

Menu::Menu()
{
	//map = NULL;
	//player = NULL;

}

Menu::~Menu()
{
	//if (map != NULL)
	//	delete map;
}

// Public functions

void Menu::init(ShaderProgram& shaderProgram)
{
	texProgram = shaderProgram;

	//spritesheet.loadFromFile("images/menu_img.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//spritesheet.setMagFilter(GL_NEAREST);
	//sprite = Sprite::createSprite(glm::ivec2(720, 405), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	mainSpritesheet.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mainSpritesheet.setMagFilter(GL_NEAREST);
	mainSprite = Sprite::createSprite(glm::ivec2(720, 405), glm::vec2(0.5f, 0.5f), &mainSpritesheet, &shaderProgram);
	
	mainSprite->setNumberAnimations(3);
	mainSprite->setAnimationSpeed(0, 1);
	mainSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	mainSprite->setAnimationSpeed(1, 1);
	mainSprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
	mainSprite->setAnimationSpeed(2, 1);
	mainSprite->addKeyframe(2, glm::vec2(0.f, 0.5f));

	mainSprite->changeAnimation(0);
	mainSprite->setPosition(glm::vec2(0.f, 0.f));
	
	helpSpriteSheet.loadFromFile("images/help_menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	helpSpriteSheet.setMagFilter(GL_NEAREST);
	helpSprite = Sprite::createSprite(glm::ivec2(720, 405), glm::vec2(0.25f, 0.5f), &helpSpriteSheet, &shaderProgram);
	helpSprite->setNumberAnimations(1);
	helpSprite->setAnimationSpeed(0, 2);
	helpSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	helpSprite->addKeyframe(0, glm::vec2(0.25f, 0.f));
	helpSprite->addKeyframe(0, glm::vec2(0.5f, 0.f));
	helpSprite->addKeyframe(0, glm::vec2(0.75f, 0.f));
	helpSprite->addKeyframe(0, glm::vec2(0.f, 0.5f));
	helpSprite->addKeyframe(0, glm::vec2(0.25f, 0.5f));
	helpSprite->addKeyframe(0, glm::vec2(0.5f, 0.5f));
	helpSprite->addKeyframe(0, glm::vec2(0.75f, 0.5f));

	helpSprite->changeAnimation(0);
	helpSprite->setPosition(glm::vec2(0.f, 0.f));

	creditsSpriteSheet.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsSpriteSheet.setMagFilter(GL_NEAREST);
	creditsSprite = Sprite::createSprite(glm::ivec2(720, 405), glm::vec2(1.f, 1.f), &creditsSpriteSheet, &shaderProgram);
	creditsSprite->setPosition(glm::vec2(0.f, 0.f));
}

void Menu::update(int deltaTime)
{
	switch (state)
	{
	case Main:
		mainSprite->update(deltaTime);
		break;
	case Help:
		helpSprite->update(deltaTime);
		break;
	case Credits:
		creditsSprite->update(deltaTime);
		break;
	default:
		break;
	}
}

void Menu::render()
{
	switch (state)
	{
	case Main:
		mainSprite->render();
		break;
	case Help:
		helpSprite->render();
		break;
	case Credits:
		creditsSprite->render();
		break;
	default:
		break;
	}
}

void Menu::setSelection(int i)
{
	mainSprite->changeAnimation(i);
}
