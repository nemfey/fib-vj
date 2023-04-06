#pragma once
#include "Enemy.h"

class Reaper : public Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;
	void setPosition(const glm::vec2& pos) override;

	bool collisionPlayer() override;

private:
	bool moveRight = true;

	bool attacking = false;
	bool attackEnded = true;

	bool renderProjectile = false;

	// Sound
	bool bChargeSoundPlayed = false;
	bool bAttackSoundPlayed = false;

	Texture projectileTex;
	Sprite* projectile;
};