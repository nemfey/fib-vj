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
	scene.init(texProgram);

	// Descomentar la linea de abajo si no se quiere ver el cursor en la pantalla
	//glutSetCursor(GLUT_CURSOR_NONE);
}

#include <iostream>

bool Game::update(int deltaTime)
{
	if (!scene.getLevelFinished())
		scene.update(deltaTime);
	else
		cout << "GAME FINISHED" << endl;
	//else if (scene.)
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderProjection();
	scene.render();
}

void Game::reshapeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
	Game::instance().updateRatioWindowSize(width, height);
}

// Input callback methods

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
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

void Game::updateRatioWindowSize(int width, int height)
{
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



