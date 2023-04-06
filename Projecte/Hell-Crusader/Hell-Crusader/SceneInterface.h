#ifndef _LEVEL_INTERFACE_INCLUDE
#define _LEVEL_INTERFACE_INCLUDE

#include <map>
#include "Sprite.h"

#include "StageState.h"

// SceneInterface contains all the entities of our game.
// It is responsible for updating and render them.


class SceneInterface
{

public:
	SceneInterface();
	~SceneInterface();

	void init(ShaderProgram &shaderProgram);
	void updateLives(int l);
	void updateScore(int s);
	void updateRemainingTime(int t);
	void updateStageNumber(int s);
	void setDeltaTime(int deltatime) { msTime += deltatime; };
	void render();

	// Gettters & Setters
	void setState(StageState s) { state = s; };
	void setLifeUp(bool b) { bLifeUp = b; };
	void setCurrentPosPlayer(glm::ivec2 p) { posPlayer = p; };

private:
	void initNumberSprite(ShaderProgram& shaderProgram);
	void initCharacterSprite(ShaderProgram& shaderProgram);
	void initHeartSprite(ShaderProgram& shaderProgram);
	void initPauseSprite(ShaderProgram& shaderProgram);
	void initLifeUpSprite(ShaderProgram& shaderProgram);
	void renderMessages();
	void renderNumber(int n, int x, int y);
	void renderCharacter(char c, int x, int y);


private:
	Texture numbersSpritesheet, charactersSpritesheet, heartSpritesheet, pauseSpriteSheet,lifeUpSpriteSheet;

		// TENEMOS UN SOLO SPRITE PARA CADA COSA Y LO VAMOS COPIANDO A LOS LADOS E IMPREMINEDO VARIAS VECES EN CADA UPDATE

	//Sprite* livesSprite, scoreSprite, remainingTimeSprite, stageSprite;
	Sprite *numberSprite;
	Sprite *characterSprite;
	Sprite *heartSprite;
	Sprite* pauseSprite;
	Sprite* lifeUpSprite;

	int lives, score, remainingTime, stageNumber;
	int msTime = 0;
	map<char, int> char2id;

	StageState state;
	
	bool bLifeUp = false;
	bool bSoundLifeUpPlaying = false;
	glm::ivec2 posPlayer;
};


#endif // _LEVEL_INTERFACE_INCLUDE

