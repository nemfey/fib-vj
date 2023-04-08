#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "SoundFactory.h"


//#define SCREEN_X 144
//#define SCREEN_Y 99
#define SCREEN_X 0
#define SCREEN_Y 48

enum playerKillers {
	LAVA, SPIKES, SAW, POISON, MUSHROOMS
};

Scene::Scene()
{
	sceneInterface = NULL;
	map = NULL;
	player = NULL;

}

Scene::~Scene()
{
	if (sceneInterface != NULL)
		delete sceneInterface;
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}

// Public functions

void Scene::init(ShaderProgram &shaderProgram, string scene)
{
	texProgram = shaderProgram;

	spritesheet.loadFromFile("images/background.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(512, 400), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setPosition(glm::vec2(0.f, 0.f));

	map = TileMap::createTileMap(scene, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//map = TileMap::createTileMap("scenes/scene28.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	sceneInterface = new SceneInterface();
	sceneInterface->init(texProgram);

	initPlayer();
	initEnemies();
	initItems();
	SoundFactory::instance().setTimeTicks(0);

	state = Starting;
	messageTimer = 2;

	bStarting = true;
	bMessageSoundPlaying = false;
	bDoorTaken = false;
	bPlayerDead = false;
	bHourglassEnding = false;
}

void Scene::update(int deltaTime)
{	
	//currentTime += deltaTime;

	//DEBUG
	//cout << currentTime << endl;

	if (state == Pause)
	{
		sceneInterface->setState(Pause);
	}
	else {
		currentTime += deltaTime;
		sceneInterface->setDeltaTime(deltaTime);
		if (bStarting)
		{
			if (!bMessageSoundPlaying)
			{
				SoundFactory::instance().playReady();
				bMessageSoundPlaying = true;
			}
			readyMessage();
			updateSceneInterface(deltaTime);
		}
		else if (bDoorTaken)
		{
			if (player->getSpawning())
				updatePlayer(deltaTime);
			else if (!player->getSpawning())
			{
				if (!bMessageSoundPlaying)
				{
					SoundFactory::instance().stopLevelMusic();
					SoundFactory::instance().playStageClear();
					bMessageSoundPlaying = true;
				}
				stageClearMessage();
				updateSceneInterface(deltaTime);
			}
		}
		else if (bPlayerDead)
		{
			SoundFactory::instance().stopImmune();
			sceneInterface->updateLives(player->getLives());
			if (!bMessageSoundPlaying)
			{
				SoundFactory::instance().stopLevelMusic();
				SoundFactory::instance().playGameOver();
				bMessageSoundPlaying = true;
			}
			gameOverMessage();
		}
		else
		{
			updatePlayer(deltaTime);
			updateEnemies(deltaTime);
			updateItems(deltaTime);
			updateSceneInterface(deltaTime);

			if (hourglassTimer > 0)
			{

				if (hourglassTimer == 1 && !bHourglassEnding) {
					SoundFactory::instance().playTimeResume();
					bHourglassEnding = true;
				}

				if (currentTime / 1000 != timer)
				{
					timer = currentTime / 1000;
					--hourglassTimer;
					//cout << "item timer is: " << hourglassTimer << endl;
				}

			}
			else {
				bHourglassEnding = false;
				SoundFactory::instance().resumeLevelMusic();
				updateTime(deltaTime);
			}
		}
	}
}

void Scene::render()
{
	sprite->render();
	
	map->render();
	for (auto i : items) {

		Treasure* pTreasure = dynamic_cast<Treasure*>(i);
		Hourglass* pHourglass = dynamic_cast<Hourglass*>(i);
		Bible* pBible = dynamic_cast<Bible*>(i);

		if (itemSpawned) {
			//50% chance of the item spawned to be a hourglass or a Bible
			if (itemRNG <= 30) {
				if (itemRNG < 15 && pHourglass) {
					if (itemCountDown > 2 || currentTime % 125 >= 50)
						pHourglass->render();
				}
				else if (itemRNG >= 15 && pBible) {
					if (itemCountDown > 2 || currentTime % 125 >= 50)
						pBible->render();
				}
			}
			else if (itemRNG > 30 && pTreasure) {
				if (itemCountDown > 2 || currentTime % 125 >= 50)
					pTreasure->render();
				pTreasure->resetType();
			}
		}
		else if (pHourglass || pTreasure || pBible)
			i->setShowing(false);
		if (!pTreasure && !pHourglass && !pBible)
			i->render();
	}

	for (auto e : enemies)
		e->render();

	if (!bStarting && (!bDoorTaken || player->getSpawning()))
		player->render();

	sceneInterface->render();
}


void Scene::makeKeyAppear()
{
	for (auto i : items)
	{
		// HAY QUE INDICAR EN EL TXT LAPOSICION DE LA LLAVE PORQUE SERA MAS FACIL
		Key* pKey = dynamic_cast<Key*>(i);
		if (pKey && !pKey->getShowing() && !pKey->getTaken())
		{
			SoundFactory::instance().playKeySpawn();
			pKey->setShowing(true);
		}
	}
}

// Private functions

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
	items.push_back(new Treasure());
	items.push_back(new Bible());

	for (auto i : items)
	{
		i->setTileMap(map);
		i->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	}
}

void Scene::readyMessage()
{
	if (SoundFactory::instance().getReadySoundFinished())
	{
		sceneInterface->setState(Playing);
		state = Playing;
		bStarting = false;
		bMessageSoundPlaying = false;

		SoundFactory::instance().playLevelMusic();
	}
}

void Scene::stageClearMessage()
{
	if (SoundFactory::instance().getStageClearSoundFinished())
	{
		if (remainingSeconds == 0)
		{
			state = StageCleared;
			bMessageSoundPlaying = false;
		}
		else
		{
			if (currentTime / 100 != timer)
			{
				timer = currentTime / 100;
				--remainingSeconds;
				player->addScore(10);
				liveScore += 10;
				SoundFactory::instance().playPointsObtained();
				score2newLive();
			}
		}
	}
}

void Scene::gameOverMessage()
{
	if (SoundFactory::instance().getGameOverSoundFinished())
	{
		state = GameOver;
		bMessageSoundPlaying = false;
	}
}

void Scene::updateTime(int deltaTime)
{
	if (remainingSeconds == 0)
	{
		sceneInterface->setState(GameOver);
		bPlayerDead = true;
		messageTimer = 5;
	}
	else if (currentTime / 1000 != timer)
	{	
		timer = currentTime / 1000;
		--remainingSeconds;
		SoundFactory::instance().playTimeTick();
		
		if (!itemSpawned) 
			--itemSpawnCounter;
		else
			--itemCountDown;

		if (itemSpawnCounter <= 0) {
			itemSpawnCounter = 20 - (rand() % 5);
			itemCountDown = 10;
			itemSpawned = true;
			itemRNG = rand() % 100;
			bItemSpawnedPlayed = false;
		}
		if (itemCountDown <= 0) {
			itemSpawned = false;
			bItemSpawnedPlayed = false;
		}

		//DEBUG
		//cout << remainingSeconds << endl;
		//if (itemSpawned)
			//cout << "Countdown: " << itemCountDown << endl;
		//else
			//cout << "SpawnTime: " << itemSpawnCounter << endl;
	}
}

void Scene::updatePlayer(int deltaTime)
{
	if (player->getRespawn())
	{
		player->resetPosition(glm::vec2(initPosPlayer.x * map->getTileSize(), initPosPlayer.y * map->getTileSize()));
		player->setRespawn(false);
		player->setSpawning(true);
	}
	
	int prevPosStepped = map->getPositionsStepped();
	player->update(deltaTime);
	int postPosStepped = map->getPositionsStepped();
	
	if (postPosStepped > prevPosStepped)
	{
		player->addScore((postPosStepped - prevPosStepped) * 10);
		liveScore += (postPosStepped - prevPosStepped) * 10;
		SoundFactory::instance().playPointsObtained();
		score2newLive();
	}

	map->setPosPlayer(player->getPosition());
}

void Scene::updateEnemies(int deltaTime)
{
	for (auto e : enemies)
	{
		if (hourglassTimer > 0)
		{
			if (hourglassTimer == 1 && !e->getIsPlayerKiller())
				e->stopwatchEnding(currentTime);
		}
		else
			e->update(deltaTime);
		
		if (e->collisionPlayer() && (!player->getInmunityState() && !bPlayerInvencible)) {
			
			if (player->getLives() > 1)
			{
				player->loseLive();
				//player->setSpawning(true);
				//player->resetPosition(glm::vec2(initPosPlayer.x * map->getTileSize(), initPosPlayer.y * map->getTileSize()));
				//map->setPosPlayer(initPosPlayer);
			}
			else
			{
				player->loseLive();
				sceneInterface->setState(GameOver);
				bPlayerDead = true;
				messageTimer = 5;
			}
		}
	}
}

void Scene::updateItems(int deltaTime)
{
	for (auto i : items)
	{
		Door* pDoor = dynamic_cast<Door*>(i);
		Treasure* pTreasure = dynamic_cast<Treasure*>(i);
		Hourglass* pHourglass = dynamic_cast<Hourglass*>(i);
		Bible* pBible = dynamic_cast<Bible*>(i);

		if (pDoor && pDoor->isTaken())
		{
			player->setSpawning(true);
			sceneInterface->setState(StageCleared);
			bDoorTaken = true;
			messageTimer = 5;

			if (player->getInmunityState() || bPlayerInvencible)
				SoundFactory::instance().stopImmune();
		}

		if (itemSpawned) {
			if (!bItemSpawnedPlayed) {
				SoundFactory::instance().playItemSpawn();
				bItemSpawnedPlayed = true;
			}

			if (pTreasure && pTreasure->collisionPlayer()) {
				SoundFactory::instance().playTreasureTaken();
				player->addScore(150 + i->getType()*150);
				liveScore += 150 + i->getType()*150;
				score2newLive();

				itemSpawnCounter = 20 - (rand() % 5);
				itemSpawned = false;
				pTreasure->setShowing(false);
				pTreasure->setPosition(glm::vec2(0, 0));
			}

			if (pBible && pBible->collisionPlayer()) {
				SoundFactory::instance().playBibleTaken();
				player->setImmune(5000, true);
				itemSpawned = false;
				pBible->setShowing(false);
				pBible->setPosition(glm::vec2(0, 0));
			}

			if (pHourglass && pHourglass->collisionPlayer()) {
				SoundFactory::instance().playTimeStop();
				SoundFactory::instance().stopLevelMusic();
				hourglassTimer = 5;
				map->setHourglassTaken(false);
				itemSpawned = false;
				pHourglass->setShowing(false);
				pHourglass->setPosition(glm::vec2(0, 0));
			}
		}

		i->update(deltaTime);
	}
}

void Scene::updateSceneInterface(int deltaTime)
{
	sceneInterface->updateLives(player->getLives());
	sceneInterface->updateScore(player->getScore());
	sceneInterface->updateRemainingTime(remainingSeconds);
}

void Scene::score2newLive()
{
	if (liveScore >= 2500)
	{
		int currentLives = player->getLives();
		
		player->setLives(currentLives + 1);
		player->setLives(currentLives + 1);
		sceneInterface->setCurrentPosPlayer(map->getPosPlayer());
		sceneInterface->setLifeUp(true);

		liveScore = liveScore - 2500;
	}
}