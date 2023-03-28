#include "SoundFactory.h"

void SoundFactory::playKeyTaken()
{
	engine->play2D("sounds/key.wav", false);
}