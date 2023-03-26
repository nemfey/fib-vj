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
	menuSpritesheet.loadFromFile("images/menu_title.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuSpritesheet.setMagFilter(GL_NEAREST);
	menuSprite = Sprite::createSprite(glm::ivec2(720, 405), glm::vec2(0.5f, 0.5f), &menuSpritesheet, &shaderProgram);
	
	menuSprite->setNumberAnimations(3);
	menuSprite->setAnimationSpeed(0, 1);
	menuSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	menuSprite->setAnimationSpeed(1, 1);
	menuSprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
	menuSprite->setAnimationSpeed(2, 1);
	menuSprite->addKeyframe(2, glm::vec2(0.f, 0.5f));

	menuSprite->changeAnimation(0);
	menuSprite->setPosition(glm::vec2(0.f, 0.f));
	
	creditsSpriteSheet.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsSpriteSheet.setMagFilter(GL_NEAREST);
	creditsSprite = Sprite::createSprite(glm::ivec2(720, 405), glm::vec2(1.f, 1.f), &creditsSpriteSheet, &shaderProgram);
	creditsSprite->setPosition(glm::vec2(0.f, 0.f));
}

void Menu::update(int deltaTime)
{
	if (bOptionsShowing)
		menuSprite->update(deltaTime);
	else
		creditsSprite->update(deltaTime);
}

void Menu::render()
{
	if (bOptionsShowing)
		menuSprite->render();
	else
		creditsSprite->render();
}

void Menu::setSelection(int i)
{
	menuSprite->changeAnimation(i);
}
