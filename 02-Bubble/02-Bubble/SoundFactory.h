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

	void playKeyTaken();

private:
	ISoundEngine* engine;
	//ISoundSource* sound;
};


#endif

