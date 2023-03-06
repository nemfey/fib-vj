#pragma once
#include "Enemy.h"

class Vampire : public Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;

private:
	void humanBehavior(int deltaTime);
	void batBehavior(int deltaTime);

private:
	enum batMovement { RightUp, LeftUp, RightDown, LeftDown };

	bool moveRight = true;
	bool humanAspect = true;
	int humanAspectTime = 0;
	int heightTransformation;
	batMovement batMov;

};

