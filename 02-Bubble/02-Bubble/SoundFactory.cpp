#include "SoundFactory.h"

void SoundFactory::setAllSoundsPaused(bool b)
{
	engine->setAllSoundsPaused(b);
}

void SoundFactory::playTimeTick()
{
	engine->play2D("sounds/time_tick.wav", false);
}

void SoundFactory::playTimeStop()
{
	engine->play2D("sounds/timestop.wav", false);
}

void SoundFactory::playTimeResume()
{
	engine->play2D("sounds/timeresume.wav", false);
}

void SoundFactory::playPlayerDie()
{
	engine->play2D("sounds/death2.wav", false);
}

void SoundFactory::playKeyTaken()
{
	engine->play2D("sounds/key.wav", false);
}

void SoundFactory::playReaperCharge()
{
	engine->play2D("sounds/charge3.wav", false);
}

void SoundFactory::playReaperAttack()
{
	engine->play2D("sounds/reaper_attack.wav", false);
}

void SoundFactory::playJump()
{
	engine->play2D("sounds/jump.wav", false);
}

void SoundFactory::playImmune()
{
	immuneSound = engine->play2D("sounds/immune.wav", true, false, true);
}

void SoundFactory::stopImmune()
{
	immuneSound->stop();
}

//void SoundFactory::stopAllSounds()
//{
//	engine->stopAllSounds();
//}
