#pragma once
#include "Item.h"

class Key : public Item
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;

private:

};

