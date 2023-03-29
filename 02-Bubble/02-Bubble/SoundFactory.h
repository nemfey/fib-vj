#ifndef _SOUND_FACTORY_INCLUDE
#define _SOUND_FACTORY_INCLUDE

#include <irrKlang.h>

using namespace irrklang;

class SoundFactory
{
private:
	SoundFactory() {
		engine = createIrrKlangDevice();
		immuneSound = engine->play2D("sounds/immune.wav", false, false, true);
	}

public:
	static SoundFactory& instance()
	{
		static SoundFactory SF;
		return SF;
	}

	void playTimeTick();
	void playTimeStop();
	void playTimeResume();
	void playPlayerDie();
	void playKeyTaken();
	void playReaperCharge();
	void playReaperAttack();
	void playJump();

	void playImmune();
	void stopImmune();


private:
	ISoundEngine* engine;

	ISound* immuneSound;
};


#endif

