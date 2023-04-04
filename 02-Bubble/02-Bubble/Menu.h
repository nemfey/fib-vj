#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "soundFactory.h"
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
	void setMusicPlaying(bool b) { bMusicPlaying = b; };
	MenuState getMenuState() { return state; };
	void setPlayerFinalScore(int i) { playerFinalScore = i; };
	void initNumberSprite(ShaderProgram& shaderProgram);
	void renderNumber(int n, int x, int y);
	void renderScore();

private:
	ShaderProgram texProgram;
	Texture mainSpritesheet, helpSpriteSheet, creditsSpriteSheet, winSpriteSheet, loseSpriteSheet, numbersSpritesheet;
	Sprite* mainSprite;
	Sprite* helpSprite;
	Sprite* creditsSprite;
	Sprite* winSprite;
	Sprite* loseSprite;
	Sprite* numberSprite;

	MenuState state = Main;

	int playerFinalScore;

	bool bMusicPlaying = false;
};


#endif // _SCENE_INCLUDE

