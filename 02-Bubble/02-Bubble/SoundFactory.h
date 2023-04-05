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
		menuMusic = engine->addSoundSourceFromFile("sounds/sewers.wav");
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

	void playMenuMusic();
	void stopMenuMusic();

	void playLevelMusic();
	void stopLevelMusic();
	void resumeLevelMusic();
	void dropLevelMusic();

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
	bool getJumpFinished();
	void playPointsObtained();
	void playLifeUp();
	void playItemSpawn();

	void playImmune();
	void stopImmune();

	void setTimeTicks(int n) { timeTicks = n; }

private:
	ISoundEngine* engine;

	ISoundSource* immuneSound;
	ISoundSource* tickSound;

	ISound* levelMusic;
	ISoundSource* menuMusic;
	
	ISound* readySound;
	ISound* gameOverSound;
	ISound* stageClearSound;
	ISound* lifeUpSound;
	ISound* itemSpawnSound;
	//ISound* jumpSound;

	ISound* transformSound;

	int timeTicks = 0;
};


#endif

