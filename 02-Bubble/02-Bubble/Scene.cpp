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

	state = Playing;
	bDoorTaken = false;
}

void Scene::update(int deltaTime)
{	
	currentTime += deltaTime;

	if (bDoorTaken)
		stageClearMessage();
	else if (player->getLives() == 0)
		gameOverMessage();
	else
	{
		updatePlayer(deltaTime);
		
		if (map->getHourglassTaken())
		{
			hourglassTimer = 5;
			map->setHourglassTaken(false);
		}
		//bool hourglassTaken = map->getHourglassTaken(); // meterlo esto en una variable de la clase y no local(?)
														// se podria tener solo el timer del hourgalss y ver si es == 5
														// si es asi esq se cogido hace poco si es 0 es que no esta cogido

		else if (hourglassTimer > 0) // esto que hace(?)
		{
			if (currentTime / 1000 != timer)
			{
				timer = currentTime / 1000;
				--hourglassTimer;
				cout << "item timer is: " << hourglassTimer << endl;
			}
			// FALTA QUE CUANDO HAY STOPWATCH NO SE COMPRUEBA SI HAY CHOQUE CON EL PLAYER
			// PROBABLEMENTE AHACER FUNCION PARA CHEQUEAR SI COLISIONES
			if (hourglassTimer == 1)
			{
				updateEnemies(deltaTime);
			}
			//if (hourglassTimer == 0)
			//{
			//	map->setHourglassTaken(false);
			//}
			//hourglassTimer = 5;
			//if (hourglassTimer == 1)
			//	bHourglassEnding = true;
		}
		else 
		{
			cout << "UPDATING" << endl;
			updateTime(deltaTime);
			updateEnemies(deltaTime);
			updateItems(deltaTime);
			updateLevelInterface(deltaTime);
		}
	}
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

void Scene::updateScene(int deltaTime)
{
	updateTime(deltaTime);

	updatePlayer(deltaTime);

	bool hourglassTaken = map->getHourglassTaken();
	if (hourglassTaken && hourglassTimer == 0)
		hourglassTimer = 5;

	
}

void Scene::stageClearMessage()
{
	// que se muestre el mensaje durante 3-5 segundos y luego state = StageClear;
}

void Scene::gameOverMessage()
{
	// que se muestre el mensaje durante 3-5 segundos y luego state = GameOver;
}

void Scene::updateTime(int deltaTime)
{
	//currentTime += deltaTime; // scarlo al principio de la funcion update

	//Previous if statement
	//if (60 - (currentTime / 1000) < remainingSeconds)

	//Game is running at 60FPS, so if the module is divisible by 60 then a second has passed
	if (currentTime / 1000 != timer)
	{	
		timer = currentTime / 1000;
		//if (!map->getHourglassTaken()) {
			--remainingSeconds;

			//DEBUG
			cout << remainingSeconds << endl;
		//}
		//else {
		//	if (hourglassTimer > 0) --hourglassTimer;
		//	if (hourglassTimer <= 0)
		//		map->setHourglassTaken(false);

			//DEBUG
		//	cout << "item timer is: " << hourglassTimer << endl;
		//}
	}

	//levelInterface->updateRemainingTime(remainingSeconds);
}

void Scene::updatePlayer(int deltaTime)
{
	int prevPosStepped = map->getPositionsStepped();
	player->update(deltaTime);
	int postPosStepped = map->getPositionsStepped();
	
	if (postPosStepped > prevPosStepped)
		levelInterface->addScore((postPosStepped - prevPosStepped) * 10); // hacer un getscore del player
																			// meter variable score en player

	map->setPosPlayer(player->getPosition());
}

void Scene::updateEnemies(int deltaTime)
{
	for (auto e : enemies)
	{
		if (hourglassTimer == 1)
			e->stopwatchEnding(currentTime);
		else
			e->update(deltaTime);

		if (e->collisionPlayer() && !player->getInmunityState())
		{
			if (player->getLives() > 1)
			{
				player->loseLive();
				//levelInterface->updateLives(player->getLives());
				player->resetPosition(glm::vec2(initPosPlayer.x * map->getTileSize(), initPosPlayer.y * map->getTileSize()));
				map->setPosPlayer(initPosPlayer);
			}
			//else
			//	// PRIMERO MOSTRAS MENSAJE GAME OVER Y LUEGO YA STATE
			//	//state = GameOver;
		}
	}
}

void Scene::updateItems(int deltaTime)
{
	for (auto i : items)
	{
		Door* d = dynamic_cast<Door*>(i);
		if (d && d->isTaken())
		{
			bDoorTaken = true;
			// PRIMERO MOSTRAR MENSAJE DE STAGE CLEARED Y LUEGO YA STATE
			//state = StageCleared;
		}
		i->update(deltaTime);
	}
}

void Scene::updateLevelInterface(int deltaTime)
{
	levelInterface->updateRemainingTime(remainingSeconds);
	levelInterface->updateLives(player->getLives());
}