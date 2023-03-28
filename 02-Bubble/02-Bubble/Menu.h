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
	void setScreenShowTime(int i) { winScreenShowTime = i; };
	void setPlayerFinalScore(int i) { playerFinalScore = i; };
	void initNumberSprite(ShaderProgram& shaderProgram);
	void renderNumber(int n, int x, int y);
	void renderScore();

private:
	void winScreenTime(int deltaTime);

private:
	ShaderProgram texProgram;
	Texture mainSpritesheet, helpSpriteSheet, creditsSpriteSheet, winSpriteSheet, numbersSpritesheet;
	Sprite* mainSprite;
	Sprite* helpSprite;
	Sprite* creditsSprite;
	Sprite* winSprite;
	Sprite* numberSprite;

	MenuState state = Main;

	int currentTime, winScreenShowTime;
	int playerFinalScore;
};


#endif // _SCENE_INCLUDE

