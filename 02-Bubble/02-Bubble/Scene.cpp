#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


//#define SCREEN_X 144
//#define SCREEN_Y 99
#define SCREEN_X 0
#define SCREEN_Y 48

enum playerKillers {
	LAVA, SPIKES, SAW, POISON, MUSHROOMS
};

Scene::Scene()
{
	levelInterface = NULL;
	map = NULL;
	player = NULL;

}

Scene::~Scene()
{
	if (levelInterface != NULL)
		delete levelInterface;
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}

// Public functions

void Scene::init(ShaderProgram &shaderProgram)
{
	texProgram = shaderProgram;
	map = TileMap::createTileMap("levels/level28.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	levelInterface = new LevelInterface();
	levelInterface->init(texProgram);

	initPlayer();
	initEnemies();
	initItems();

	remainingSeconds, currentTime = 60;
	timer = 0;
	bLevelFinished = false;
}

void Scene::update(int deltaTime)
{	
	updateTime(deltaTime);
	
	int prevPosStepped = map->getPositionsStepped();
	player->update(deltaTime);
	int postPosStepped = map->getPositionsStepped();
	if (postPosStepped > prevPosStepped)

		levelInterface->addScore((postPosStepped - prevPosStepped) * 10);

	//cout << "pos player: " << player->getPosition().x << " " << player->getPosition().y << endl;;

	map->setPosPlayer(player->getPosition());

	bool hourglassTaken = map->getHourglassTaken();
	if (hourglassTaken && hourglassTimer == 0)
		hourglassTimer = 5;

	for (auto e : enemies)
	{
		if (!hourglassTaken || e->getIsPlayerKiller())
			e->update(deltaTime);
		else if (hourglassTimer == 1)
			e->stopwatchEnding(currentTime);

		if (e->collisionPlayer() && !player->getInmunityState())
		{
			player->loseLive();
			levelInterface->updateLives(player->getLives());
			player->resetPosition(glm::vec2(initPosPlayer.x * map->getTileSize(), initPosPlayer.y * map->getTileSize()));
			map->setPosPlayer(initPosPlayer);
		}
	}

	for (auto i : items)
	{
		Door* d = dynamic_cast<Door*>(i);
		if (d && d->isTaken())
		{
			bLevelFinished = true;
		}
		i->update(deltaTime);
	}
	
	//else
	//{
	//	levelInterface->setStageClear(true);
	//}
}

void Scene::render()
{
	map->render();
	for (auto i : items)
		i->render();
	for (auto e : enemies)
		e->render();
	player->render();
	levelInterface->render();
}

void Scene::initPlayer()
{
	player = new Player();
	initPosPlayer = map->getPosPlayer();

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(initPosPlayer.x * map->getTileSize(), initPosPlayer.y * map->getTileSize()));
	player->setTileMap(map);
}

void Scene::initEnemies()
{
	vector<std::pair<char, glm::ivec2>> tileMapEnemies = map->getEnemies();

	for (unsigned int i = 0; i < tileMapEnemies.size(); ++i) {
		if (tileMapEnemies[i].first == 'S') {
			enemies.push_back(new Skeleton());
		}
		else if(tileMapEnemies[i].first == 'V')
		{
			enemies.push_back(new Vampire());
		}
		else if (tileMapEnemies[i].first == 'R')
		{
			enemies.push_back(new Reaper());
		}
		else if (tileMapEnemies[i].first == 'X')
		{
			enemies.push_back(new PlayerKiller(LAVA));
		}
		else if (tileMapEnemies[i].first == 'x')
		{
			enemies.push_back(new PlayerKiller(SAW));
		}
		auto e = enemies[i];
		e->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		e->setPosition(glm::vec2(tileMapEnemies[i].second[0] * map->getTileSize(), tileMapEnemies[i].second[1] * map->getTileSize()));
		e->setTileMap(map);
	}
}

void Scene::initItems()
{
	items.push_back(new Key());
	items.push_back(new Door());
	items.push_back(new Hourglass());

	for (auto i : items)
	{
		i->setTileMap(map);
		i->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	}
}

void Scene::updateTime(int deltatime)
{
	currentTime += deltatime;

	//Previous if statement
	//if (60 - (currentTime / 1000) < remainingSeconds)

	//Game is running at 60FPS, so if the module is divisible by 60 then a second has passed
	if (currentTime / 1000 != timer)
	{	
		timer = currentTime / 1000;
		if (!map->getHourglassTaken()) {
			--remainingSeconds;

			//DEBUG
			cout << remainingSeconds << endl;
		}
		else {
			if (hourglassTimer > 0) --hourglassTimer;
			if (hourglassTimer <= 0)
				map->setHourglassTaken(false);

			//DEBUG
			cout << "item timer is: " << hourglassTimer << endl;
		}
	}

	levelInterface->updateRemainingTime(remainingSeconds);
}
