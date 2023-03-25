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
	spritesheet.loadFromFile("images/menu_buttons.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(720, 405), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	
	sprite->setNumberAnimations(3);
	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->setAnimationSpeed(1, 1);
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
	sprite->setAnimationSpeed(2, 1);
	sprite->addKeyframe(2, glm::vec2(0.f, 0.5f));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(0.f, 0.f));
	//map = TileMap::createTileMap("levels/level28.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	//projection = glm::ortho(0.f, float(windowSize.x - 1), float(windowSize.y - 1), 0.f);
	//projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	//currentTime = 0;
}

void Menu::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Menu::render()
{
	sprite->render();
}

void Menu::setSelection(int i)
{
	sprite->changeAnimation(i);
}
