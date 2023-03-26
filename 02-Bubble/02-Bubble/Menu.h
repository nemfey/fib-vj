#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MenuState.h"


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
	void setSelection(int i);
	void setMenuState(MenuState m) { state = m; };

private:
	ShaderProgram texProgram;
	Texture mainSpritesheet, helpSpriteSheet, creditsSpriteSheet;
	Sprite* mainSprite;
	Sprite* helpSprite;
	Sprite* creditsSprite;

	MenuState state = Main;
};


#endif // _SCENE_INCLUDE

