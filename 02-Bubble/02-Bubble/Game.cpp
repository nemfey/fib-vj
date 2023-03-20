#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"

// Public functions

void Game::init()
{	
	initShaders();
	bPlay = true;
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	menu.init(texProgram);

	bMenuShowing = true;
	// Descomentar la linea de abajo si no se quiere ver el cursor en la pantalla
	//glutSetCursor(GLUT_CURSOR_NONE);
}

#include <iostream>

bool Game::update(int deltaTime)
{
	if (bMenuShowing)
		updateMenu(deltaTime);
	else
		updateScene(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderProjection();

	if (bMenuShowing)
		menu.render();
	else
		scene->render();
}

void Game::reshapeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
	Game::instance().updateRatioWindowSize(width, height);
}

// Input callback methods

void Game::keyPressed(int key)
{
	cout << "key: " << key << endl;
	if (key == 27) // Escape code
		bPlay = false;
	if (key == 13 && bMenuShowing) // Enter code
		loadScene("levels/level01.txt");
	if (key==49)
		loadScene("levels/level01.txt");
	if (key==50)
		loadScene("levels/level02.txt");
	if (key == 51)
		loadScene("levels/level03.txt");
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

// Getters & Setters
bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

// Private functions

void Game::initShaders()
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

void Game::renderProjection()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
}

void Game::updateMenu(int deltaTime)
{
	menu.update(deltaTime);
}

void Game::updateScene(int deltaTime)
{
	switch (scene->state)
	{
	case StageState::Playing:
		scene->update(deltaTime);
		break;
	case StageState::StageCleared:
		// load next stage and if last end game
		loadScene("levels/level02.txt");
		cout << "loading next stage..." << endl;
		break;
	case StageState::GameOver:
		// Show menu again
		bMenuShowing = true;
		cout << "Going back to menu..." << endl;
		break;
	default:
		break;
	}
}

void Game::updateRatioWindowSize(int width, int height)
{
	float windowRatio = width / float(height);
	float scale = 1.0f;
	float currentRatio = bMenuShowing ? menuRatio : sceneRatio;

	if (windowRatio > currentRatio)
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

void Game::loadScene(string level)
{
	scene = new Scene();
	//scene->init(texProgram, "levels/level28.txt");
	scene->init(texProgram, level);
	bMenuShowing = false;
}

