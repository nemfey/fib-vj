#include "SoundFactory.h"
#include <iostream>
using namespace std;

void SoundFactory::setAllSoundsPaused(bool b)
{
	engine->setAllSoundsPaused(b);
}

void SoundFactory::playMenuMusic()
{
	menuMusic->setDefaultVolume(0.6);
	engine->play2D(menuMusic, true);
}

void SoundFactory::stopMenuMusic()
{
	engine->stopAllSoundsOfSoundSource(menuMusic);
}

void SoundFactory::playLevelMusic()
{
	//levelMusic = engine->play2D("sounds/castle3.wav", false, false, true);
	if (levelMusic == nullptr) {
		levelMusic = engine->play2D("sounds/castle3.wav", false, false, true);
		levelMusic->setVolume(0.4);
	}
	else {
		levelMusic->setPlayPosition(0);
	}
}

void SoundFactory::stopLevelMusic()
{
	if (levelMusic != nullptr)
		levelMusic->setIsPaused(true);
}

void SoundFactory::resumeLevelMusic()
{
	levelMusic->setIsPaused(false);
}

void SoundFactory::dropLevelMusic()
{
	if (levelMusic != nullptr)
		levelMusic->drop();
}

void SoundFactory::playChangeOption()
{
	engine->play2D("sounds/select1.wav", false);
}

void SoundFactory::playSelectOption()
{
	engine->play2D("sounds/select2.wav", false);
}

void SoundFactory::playTimeTick()
{
	if (timeTicks < 10)
		tickSound->setDefaultVolume(0.05);
	if (timeTicks >= 10)
		tickSound->setDefaultVolume(0.1);
	if (timeTicks >= 20)
		tickSound->setDefaultVolume(0.2);
	if (timeTicks >= 30)
		tickSound->setDefaultVolume(0.5);
	if (timeTicks >= 40)
		tickSound->setDefaultVolume(0.7);
	if (timeTicks >= 50)
		tickSound->setDefaultVolume(1.0);
	engine->play2D(tickSound);
	++timeTicks;
}

void SoundFactory::playReady()
{
	readySound = engine->play2D("sounds/ready.wav", false, false, true);
	//readySound->drop();
}

void SoundFactory::playGameOver()
{
	gameOverSound = engine->play2D("sounds/game_over.wav", false, false, true);
	//gameOverSound->drop();
}

void SoundFactory::playStageClear()
{
	stageClearSound = engine->play2D("sounds/stage_clear.wav", false, false, true);
	//stageClearSound->drop();
}

void SoundFactory::playTimeStop()
{
	ISound* sound = engine->play2D("sounds/timestop.wav", false, false, true);
	sound->setPlayPosition(500);
	//sound->drop();
}

void SoundFactory::playTimeResume()
{
	engine->play2D("sounds/timeresume.wav", false);
}

void SoundFactory::playPlayerDie()
{
	engine->play2D("sounds/death2.wav", false);
}

void SoundFactory::playBibleTaken()
{
	ISound* sound = engine->play2D("sounds/bible.wav", false, false, true);
	sound->setVolume(0.5f);
}

void SoundFactory::playTreasureTaken()
{
	engine->play2D("sounds/prize2.wav", false);
}

void SoundFactory::playKeyTaken()
{
	engine->play2D("sounds/key.wav", false);
}

void SoundFactory::playKeySpawn()
{
	engine->play2D("sounds/key_spawn.wav", false);
}

void SoundFactory::playVampireTransform()
{
	transformSound = engine->play2D("sounds/transform2.wav", false, false, true);
	transformSound->setVolume(0.5);
}

void SoundFactory::playVampireUntransform()
{
	transformSound = engine->play2D("sounds/transform1.wav", false, false, true);
	transformSound->setVolume(0.5);
}

void SoundFactory::playReaperCharge()
{
	engine->play2D("sounds/charge3.wav", false);
}

void SoundFactory::playReaperAttack()
{
	engine->play2D("sounds/reaper_attack.wav", false);
}

void SoundFactory::playPlayerSpawn()
{
	ISound* sound = engine->play2D("sounds/spawn.wav", false, false, true);
	sound->setVolume(0.3f);
	//sound->drop();
}

void SoundFactory::playJump()
{
	if (!engine->isCurrentlyPlaying("sounds/jump2.wav")) {
		ISound* sound = engine->play2D("sounds/jump2.wav", false, false, true);
		sound->setVolume(0.3f);
	}
	//sound->drop();
}

bool SoundFactory::getJumpFinished()
{
	return !engine->isCurrentlyPlaying("sounds/jump2.wav");
}

void SoundFactory::playPointsObtained()
{
	engine->play2D("sounds/point_low.wav", false);
}

void SoundFactory::playLifeUp()
{ 
	lifeUpSound = engine->play2D("sounds/life_up.wav", false, false, true);
	lifeUpSound->setVolume(0.3f);
	//lifeUpSound->drop();
}

void SoundFactory::playItemSpawn()
{
	itemSpawnSound = engine->play2D("sounds/charge1.wav", false, false, true);
	itemSpawnSound->setVolume(0.3f);
}

void SoundFactory::playImmune()
{
	engine->play2D(immuneSound, true);
}

void SoundFactory::stopImmune()
{
	engine->stopAllSoundsOfSoundSource(immuneSound);
}

//void SoundFactory::stopAllSounds()
//{
//	engine->stopAllSounds();
//}
