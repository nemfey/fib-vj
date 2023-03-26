#pragma once
#include "Item.h"
#include <irrKlang.h>
using namespace irrklang;

class Key : public Item
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void update(int deltaTime) override;
	void render() override;

private:
	bool bPositioned = false;

	//Sound engine
	ISoundEngine* engine = createIrrKlangDevice();
	ISound* sound;
};

