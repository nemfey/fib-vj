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
	void transformationProcess();
	void checkMovingBehavior();

private:
	enum batMovement { RightUp, LeftUp, RightDown, LeftDown };

	bool bMoveRight = true;
	bool bHumanAspect = true;
	int humanAspectTime = 0;
	int heightTransformation;
	bool bTransformation = false;
	batMovement batMov;

};

