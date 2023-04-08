#pragma once
#include "Enemy.h"

class PlayerKiller : public Enemy
{

public:
	PlayerKiller(int id);

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;

	bool collisionPlayer() override;

	void setType(int type);
	int getType();

private:
	int type = 0;
};