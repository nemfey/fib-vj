#ifndef _SOUND_FACTORY_INCLUDE
#define _SOUND_FACTORY_INCLUDE

#include <irrKlang.h>

using namespace irrklang;

class SoundFactory
{
private:
	SoundFactory() {
		engine = createIrrKlangDevice();
	}

public:
	static SoundFactory& instance()
	{
		static SoundFactory SF;
		return SF;
	}

	void playTimeTick();
	void playTimeStop();
	void playPlayerDie();
	void playKeyTaken();
	void playReaperCharge();
	void playReaperAttack();


private:
	ISoundEngine* engine;
	//ISoundSource* sound;
};


#endif

