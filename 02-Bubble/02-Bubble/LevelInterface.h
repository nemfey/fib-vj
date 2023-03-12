#ifndef _LEVEL_INTERFACE_INCLUDE
#define _LEVEL_INTERFACE_INCLUDE


#include <glm/glm.hpp>
#include "Sprite.h"


// LevelInterface contains all the entities of our game.
// It is responsible for updating and render them.


class LevelInterface
{

public:
	LevelInterface();
	~LevelInterface();

	void init(ShaderProgram& shaderProgram);
	void addOneLive();
	void minusOneLive();
	void addScore(int s);
	void updateRemainingTime(int t);
	void updateStage(int s);
	void render();

	// Gettters & Setters


private:
	Texture numbersSpritesheet, wordsSpritesheet;


	Sprite* livesSprite;
	vector<Sprite*> scoreSprites;
	vector<Sprite*> remainingTimeSprites;
	vector<Sprite*> stageSprites;
	

	int lives;
	int score;
	int remainingTime;
	int stage;
};


#endif // _LEVEL_INTERFACE_INCLUDE

