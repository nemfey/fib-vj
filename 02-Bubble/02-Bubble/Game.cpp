#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

// Public functions

void Game::init()
{	
	bPlay = true;
	//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	scene.updateRatioWindowSize(width, height);
}

void Game::reshapeCallback(int width, int height)
{
	Game::instance().reshape(width, height);
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





