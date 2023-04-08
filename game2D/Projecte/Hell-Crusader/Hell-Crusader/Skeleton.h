#pragma once
#include "Enemy.h"

class Skeleton : public Enemy 
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void setPosition(const glm::vec2& pos) override;

private:
	bool moveRight = true;

};

