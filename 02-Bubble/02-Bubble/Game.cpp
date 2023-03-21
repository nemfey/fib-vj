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

	dictOptions = { {0,Play}, {1,Instructions}, {2,Credits}, {3,Exit} };
	option_nth = 0;
	
	stages.push_back("levels/level01.txt");
	stages.push_back("levels/level02.txt");
	stages.push_back("levels/level03.txt");
	stageIterator = 0;
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
	if (key == 27) // Escape code
		bPlay = false;
	if (key == 13 && bMenuShowing) // Enter code
		optionSelected();
	if (key == 49)
	{
		stageIterator = 0;
		loadFirstStage();
	}
	if (key == 50)
	{
		stageIterator = 1;
		loadFirstStage();
	}
	if (key == 51)
	{
		stageIterator = 2;
		loadFirstStage();
	}
	if (key == 103)
	{
		if (!scene->getPlayerInvencible())
		{
			cout << "Entering invencible mode..." << endl;
			scene->setPlayerInvencible(true);
		}
		else
		{
			cout << "Exiting invencible mode..." << endl;
			scene->setPlayerInvencible(false);
		}
	}
	if (key == 107)
		scene->makeKeyAppear();

	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	if (key == 101 && option_nth > 0)
	{
		--option_nth;
		cout << "opcion " << option_nth << endl;
		//menu change animation options[option_nth]
		// cambiar sprite al que diga el diccionario
		// para que se muestre el sprite del menu con dicho boton marcado
	}
	if (key == 103 && option_nth < 3)
	{
		++option_nth;
		cout << "opcion " << option_nth << endl;
		//menu change animation options[option_nth]
		// cambiar sprite al que diga el diccionario
		// para que se muestre el sprite del menu con dicho boton marcado
	}

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
		++stageIterator;
		if (stageIterator < stages.size())
			loadNextStage();
		else
			cout << "GAME FINISHED. CONGRATULATIONS!" << endl;
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

void Game::loadFirstStage()
{
	scene = new Scene();
	scene->init(texProgram, stages[stageIterator]);
	bMenuShowing = false;

	scene->setStageNumber(stageIterator + 1);
}

void Game::loadNextStage()
{
	int playerLives = scene->getPlayerLives();
	int playerScore = scene->getPlayerScore();

	scene = new Scene();
	scene->init(texProgram, stages[stageIterator]);
	bMenuShowing = false;
	
	scene->setPlayerLives(playerLives);
	scene->setPlayerScore(playerScore);
	scene->setStageNumber(stageIterator + 1);
}

void Game::optionSelected()
{
	cout << dictOptions[option_nth] << endl;
	switch (dictOptions[option_nth])
	{
	case Play:
		cout << "playing..." << endl;
		stageIterator = 0;
		loadFirstStage();
		break;
	case Instructions:
		// Mostrar instrucciones
		cout << "showing intructions..." << endl;
		break;
	case Credits:
		// Mostrar pantalla de creditos
		cout << "showing credits..." << endl;
		break;
	case Exit:
		cout << "Exiting..." << endl;
		bPlay = false;
		break;
	default:
		break;
	}

}

