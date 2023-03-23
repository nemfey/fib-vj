#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include <vector>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram &program);
	TileMap();
	~TileMap();

	void render() const;
	void free();

	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, bool bCrossPlattforms) const;
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, bool bCrossPlattforms) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool bCrossPlattforms) const;
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool allPlattformsStepped() const;
	bool doorOpen();
	glm::vec2 getRandomPosition(int radius);
	void positionStepped(const glm::ivec2& pos, const glm::ivec2& size, int* posY);

	
	// Getters & Setters
	int getTileSize() const { return tileSize; }
	glm::ivec2 getPosPlayer() const { return posPlayer; };
	int getPositionsStepped() const { return positionsStepped.size(); };
	vector<pair<char, glm::ivec2>> getEnemies() const { return enemies; };
	glm::ivec2 getDoorPosition() { return posDoor; };
	bool getKeyTaken() const { return keyTaken; };
	bool getHourglassTaken() const { return hourglassTaken; };
	bool getBibleTaken() const { return bibleTaken; }
	glm::ivec2 getMapSize() const { return mapSize; };
	glm::ivec2 getScreenCoords() const { return screenCoords; };

	void setPosPlayer(glm::ivec2 pos) { posPlayer = pos; };
	void setKeyTaken(bool b) { keyTaken = b; };
	void setHourglassTaken(bool b);
	void setBibleTaken(bool b) { bibleTaken = b; }


private:
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	int* map;
	
	GLuint vao, vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize, screenCoords;
	int nTiles, tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	
	Sprite* steppedFloorSprite;
	vector<glm::ivec2> positionsStepped;
	vector<glm::ivec2> plattforms;
	int nStepTiles = 0;

	glm::ivec2 posPlayer;
	vector<pair<char, glm::ivec2>> enemies;

	glm::ivec2 posDoor = glm::ivec2(0,0);
	bool keyTaken = false;

	bool hourglassTaken = false;

	bool bibleTaken = false;

};


#endif // _TILE_MAP_INCLUDE