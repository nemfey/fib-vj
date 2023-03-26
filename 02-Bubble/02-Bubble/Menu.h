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
	void setSelection(int i);
	void setOptionsShowing(bool b) { bOptionsShowing = b; };

private:
	ShaderProgram texProgram;
	Texture menuSpritesheet, creditsSpriteSheet;
	Sprite* menuSprite;
	Sprite* creditsSprite;

	bool bOptionsShowing = true;
};


#endif // _SCENE_INCLUDE

