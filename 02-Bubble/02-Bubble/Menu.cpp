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

void Menu::init()
{
	initShaders();
	//map = TileMap::createTileMap("levels/level28.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	//projection = glm::ortho(0.f, float(windowSize.x - 1), float(windowSize.y - 1), 0.f);
	//projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0;
}

void Menu::update(int deltaTime)
{
	updateTime(deltaTime);
}

void Menu::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	//map->render();
}

void Menu::updateRatioWindowSize(int width, int height)
{
	float gameRatio = 1.28f;
	float windowRatio = width / float(height);
	float scale = 1.0f;

	if (windowRatio > gameRatio)
	{
		scale = height / 400.0f;  // escalate Y axis
		float offset = (width - 512 * scale) / 2;
		projection = glm::ortho(-offset, float(width) - offset, float(height), 0.f);
	}
	else
	{
		scale = width / 512.0f;  // escalate X axis
		float offset = (height - 400 * scale) / 2;
		projection = glm::ortho(0.f, float(width), float(height) - offset, -offset);
	}
	projection = glm::scale(projection, glm::vec3(scale));
}

// Private functions

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Menu::updateTime(int deltatime)
{
	currentTime += deltatime;
}
