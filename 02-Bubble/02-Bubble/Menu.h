#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"


// Menu contains all the entities of our game.
// It is responsible for updating and render them.


class Menu
{

public:
	Menu();
	~Menu();

	void init(ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

private:
	ShaderProgram texProgram;
	Texture spritesheet;
	Sprite* sprite;
};


#endif // _SCENE_INCLUDE

