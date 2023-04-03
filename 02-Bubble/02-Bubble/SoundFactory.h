#ifndef _SOUND_FACTORY_INCLUDE
#define _SOUND_FACTORY_INCLUDE

#include <irrKlang.h>

using namespace irrklang;

class SoundFactory
{
private:
	SoundFactory() {
		engine = createIrrKlangDevice();
		immuneSound = engine->addSoundSourceFromFile("sounds/immune.wav");
		tickSound = engine->addSoundSourceFromFile("sounds/time_tick.wav");
		timeTicks = 0;
	}

public:
	static SoundFactory& instance()
	{
		static SoundFactory SF;
		return SF;
	}

	void setAllSoundsPaused(bool b);
	//void stopAllSounds(bool b);

	void playLevelMusic();
	void stopLevelMusic();
	void resumeLevelMusic();

	void playChangeOption();
	void playSelectOption();
	void playTimeTick();

	void playReady();
	bool getReadySoundFinished() { return readySound->isFinished(); };
	void playGameOver();
	bool getGameOverSoundFinished() { return gameOverSound->isFinished(); };
	void playStageClear();
	bool getStageClearSoundFinished() { return stageClearSound->isFinished(); };
	bool getLifeUpSoundFinished() { return lifeUpSound->isFinished(); };

	void playTimeStop();
	void playTimeResume();
	void playPlayerDie();
	void playBibleTaken();
	void playTreasureTaken();
	void playKeyTaken();
	void playKeySpawn();
	void playVampireTransform();
	void playVampireUntransform();
	void playReaperCharge();
	void playReaperAttack();
	void playPlayerSpawn();
	void playJump();
	void playPointsObtained();
	void playLifeUp();
	void playItemSpawned();

	void playImmune();
	void stopImmune();

	void setTimeTicks(int n) { timeTicks = n; }

private:
	ISoundEngine* engine;

	ISoundSource* immuneSound;
	ISoundSource* tickSound;

	ISound* levelMusic;
	
	ISound* readySound;
	ISound* gameOverSound;
	ISound* stageClearSound;
	ISound* lifeUpSound;

	int timeTicks = 0;
};


#endif

