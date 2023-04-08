#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <irrKlang.h>
#include "Sprite.h"
#include "TileMap.h"

using namespace irrklang;


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
	bool getInmunityState() const { return inmunityState || inmunityStateBible; };
	int getLives() const { return lives; };
	int getScore() const { return score; };
	bool getRespawn() { return bRespawn; };
	bool getSpawning() { return bSpawning; };
	bool getDying() { return bDying; }

	void setTileMap(TileMap* tileMap) { map = tileMap; };
	void setPosition(const glm::ivec2 &pos);
	void setImmune(int time, bool bible);
	void setLives(int l) { lives = l; };
	void setInvincible(bool b) { bInvincible = b; }
	void setRespawn(bool b) { bRespawn = b; };
	void setSpawning(bool b) { bSpawning = b; };
	
private:
	Texture spritesheet;
	Sprite* sprite;
	
	glm::ivec2 tileMapDispl, posPlayer;
	TileMap *map;

	bool bJumping;
	int jumpAngle, startY;

	int lives = 3;
	bool inmunityState = false;
	bool inmunityStateBible = false;
	int inmunityTime = 0;

	int score = 0;

	bool bInvincible = false;

	bool bFacingRight = true;

	bool bDying = false;
	bool bRespawn = false;
	bool bSpawning = true;

	bool bImmuneSoundPlayed = false;
	bool bJumpSoundPlayed = false;
	bool bSpawnSoundPlayed = false;
};


#endif // _PLAYER_INCLUDE


