#pragma once
#include "Enemy.h"

class Reaper : public Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;

private:
	bool moveRight = true;

	bool attacking = false;
	bool attackEnded = true;

};