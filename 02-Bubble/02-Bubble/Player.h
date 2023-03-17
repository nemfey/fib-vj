#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	~Player();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void loseLive();
	void resetPosition(const glm::ivec2& pos);
	void addScore(int s);
	
	// Getters & Setters
	glm::ivec2 getPosition() const { return posPlayer; };
	bool getInmunityState() const { return inmunityState; };
	int getLives() const { return lives; };
	int getScore() const { return score; };

	void setTileMap(TileMap* tileMap) { map = tileMap; };
	void setPosition(const glm::ivec2 &pos);
	
private:
	Texture spritesheet;
	Sprite* sprite;
	
	glm::ivec2 tileMapDispl, posPlayer;
	TileMap *map;

	bool bJumping;
	int jumpAngle, startY;

	int lives = 3;
	bool inmunityState = false;
	int inmunityTime = 0;

	int score = 0;

};


#endif // _PLAYER_INCLUDE


