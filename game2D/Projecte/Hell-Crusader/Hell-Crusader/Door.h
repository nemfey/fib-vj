#pragma once
#include "Item.h"

class Door : public Item
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;

private:
	void openingProcess();


private:
	bool bOpening = false;

};