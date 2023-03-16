#pragma once
#include "Item.h"

class Treasure : public Item
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;

	int getType();

private:

	int type = 0;
};