#pragma once
#include "Enemy.h"
#include <irrKlang.h>
using namespace irrklang;

class Reaper : public Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;

	bool collisionPlayer() override;

private:
	bool moveRight = true;

	bool attacking = false;
	bool attackEnded = true;

	bool renderProjectile = false;

	//Sound engine
	ISoundEngine* engine = createIrrKlangDevice();
	ISound* sound;
	bool bChargeSoundPlayed = false;
	bool bAttackSoundPlayed = false;

	Texture projectileTex;
	Sprite* projectile;
};