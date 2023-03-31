#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "SoundFactory.h"

// Public functions

void Game::init()
{
	initShaders();
	bPlay = true;
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	//glClearColor(0.f, 0.f, 0.f, 1.0f);
	menu.init(texProgram);

	bMenuShowing = true;

	dictOptions = { {0,Play}, {1,Instructions}, {2,Exit} };
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
	{
		menu.render();
		updateRatioWindowSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	else
		scene->render();
}

void Game::reshapeCallback(int width, int height)
{
	Game::instance().updateRatioWindowSize(width, height);
}

// Input callback methods

void Game::keyPressed(int key)
{
	cout << key << endl;
	if (key == 105) {
		scene->setItemSpawned(false);
		scene->setItemSpawnCounter(0);
	}
	if (key == 27 && (!bMenuShowing || menu.getMenuState() != Main)) // Escape code
	{
		SoundFactory::instance().setAllSoundsPaused(true);
		bMenuShowing = true;
		SoundFactory::instance().playSelectOption();
		menu.setMenuState(Main);
	}
	if ((key == 13 || key == 32) && bMenuShowing) // Enter code
	{
		SoundFactory::instance().playSelectOption();
		optionSelected();
	}
	if (key == 49)
	{
		SoundFactory::instance().setAllSoundsPaused(true);
		stageIterator = 0;
		loadFirstStage();
	}
	if (key == 50)
	{
		SoundFactory::instance().setAllSoundsPaused(true);
		stageIterator = 1;
		loadFirstStage();
	}
	if (key == 51)
	{
		SoundFactory::instance().setAllSoundsPaused(true);
		stageIterator = 2;
		loadFirstStage();
	}
	if ((key == 99 || key == 69) && menu.getMenuState() == Main)
	{
		SoundFactory::instance().playSelectOption();
		menu.setMenuState(Credits);
	}
	if ((key == 103 || key == 71) && !bMenuShowing)
	{
		if (!scene->getPlayerInvencible())
			scene->setPlayerInvencible(true);
		else
			scene->setPlayerInvencible(false);
	}
	if ((key == 107 || key == 75) && !bMenuShowing)
	{
		scene->makeKeyAppear();
	}
	if (key == 112 && !bMenuShowing)
	{
		StageState s = scene->state;
		if (s == Playing)
		{
			scene->state = Pause;
			SoundFactory::instance().setAllSoundsPaused(true);
		}
		else if (s == Pause)
		{
			scene->state = Playing;
			scene->setSceneInterfaceState(Playing);
			SoundFactory::instance().setAllSoundsPaused(false);
		}
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	if (key == 101 && option_nth > 0 && bMenuShowing)
	{
		--option_nth;
		SoundFactory::instance().playChangeOption();
		menu.setSelection(option_nth);
	}
	if (key == 103 && option_nth < 2 && bMenuShowing)
	{
		++option_nth;
		SoundFactory::instance().playChangeOption();
		menu.setSelection(option_nth);
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
	case StageState::Starting:
		scene->update(deltaTime);
		break;
	case StageState::Playing:
		scene->update(deltaTime);
		break;
	case StageState::Pause:
		scene->update(deltaTime);
		break;
	case StageState::StageCleared:
		++stageIterator;
		if (stageIterator < int(stages.size()))
			loadNextStage();
		else
		{
			menu.setPlayerFinalScore(scene->getPlayerScore());
			menu.setMenuState(Win);
			bMenuShowing = true;
		}
		break;
	case StageState::GameOver:
		menu.setPlayerFinalScore(scene->getPlayerScore());
		menu.setMenuState(Lose);
		bMenuShowing = true;
		break;
	default:
		break;
	}
}

void Game::updateRatioWindowSize(int width, int height)
{
	glViewport(0, 0, width, height);

	float windowRatio = width / float(height);
	float scale = 1.0f;
	float currentRatio = bMenuShowing ? menuRatio : sceneRatio;
	float currentWidth = bMenuShowing ? 405.f : 400.f;
	float currentHeight = bMenuShowing ? 720.f : 512.f;

	if (windowRatio > currentRatio)
	{
		scale = height / currentWidth;
		float offset = (width - currentHeight * scale) / 2;
		projection = glm::ortho(-offset, float(width) - offset, float(height), 0.f);
	}
	else
	{
		scale = width / currentHeight;
		float offset = (height - currentWidth * scale) / 2;
		projection = glm::ortho(0.f, float(width), float(height) - offset, -offset);
	}
	projection = glm::scale(projection, glm::vec3(scale));
}

void Game::loadFirstStage()
{
	SoundFactory::instance().stopImmune();

	scene = new Scene();
	scene->init(texProgram, stages[stageIterator]);
	
	bMenuShowing = false;
	updateRatioWindowSize(glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT));

	scene->setStageNumber(stageIterator + 1);
}

void Game::loadNextStage()
{
	int playerLiveScore = scene->getLiveScore();
	int playerLives = scene->getPlayerLives();
	int playerScore = scene->getPlayerScore();

	scene = new Scene();
	scene->init(texProgram, stages[stageIterator]);
	bMenuShowing = false;
	
	scene->setLiveScore(playerLiveScore);
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
		stageIterator = 0;
		loadFirstStage();
		break;
	case Instructions:
		menu.setMenuState(Help);
		break;
	case Exit:
		bPlay = false;
		break;
	default:
		break;
	}

}

