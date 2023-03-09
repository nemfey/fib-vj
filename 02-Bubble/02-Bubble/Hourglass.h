#pragma once
#include "Item.h"

class Hourglass : public Item
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;

private:

};
