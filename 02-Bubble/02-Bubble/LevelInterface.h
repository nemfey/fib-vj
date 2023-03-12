#ifndef _LEVEL_INTERFACE_INCLUDE
#define _LEVEL_INTERFACE_INCLUDE


#include "Sprite.h"


// LevelInterface contains all the entities of our game.
// It is responsible for updating and render them.


class LevelInterface
{

public:
	LevelInterface();
	~LevelInterface();

	void init(ShaderProgram& shaderProgram);
	void updateLives(int l);
	void addScore(int s);
	void updateRemainingTime(int t);
	void updateStage(int s);
	void render();

	// Gettters & Setters

private:
	void initNumberSprite(ShaderProgram& shaderProgram);
	void initCharacterSprite(ShaderProgram& shaderProgram);
	void initHeartSprite(ShaderProgram& shaderProgram);


private:
	Texture numbersSpritesheet, charactersSpritesheet, heartSpritesheet;

		// TENEMOS UN SOLO SPRITE PARA CADA COSA Y LO VAMOS COPIANDO A LOS LADOS E IMPREMINEDO VARIAS VECES EN CADA UPDATE

	//Sprite* livesSprite, scoreSprite, remainingTimeSprite, stageSprite;
	Sprite *numberSprite;
	Sprite *characterSprite;
	Sprite *heartSprite;

	int lives, score, remainingTime, stage;
};


#endif // _LEVEL_INTERFACE_INCLUDE

