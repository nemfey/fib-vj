#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"

#include <Windows.h>
#include <thread>

using namespace std;


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
	screenCoords = minCoords;

	steppedFloorSprite = Sprite::createSprite(glm::ivec2(tileSize, tileSize), glm::vec2(0.25, 0.5), &tilesheet, &program);
	steppedFloorSprite->setNumberAnimations(2);
	steppedFloorSprite->addKeyframe(0, glm::vec2(0.5f, 0.f));
	steppedFloorSprite->addKeyframe(1, glm::vec2(0.75f, 0.f));
}

// Public functions
TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram &program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);
	return map;
}

TileMap::TileMap()
{
	map = NULL;
	steppedFloorSprite = NULL;

}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
	if (steppedFloorSprite != NULL)
		delete steppedFloorSprite;
}

void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);

	for (auto p : positionsStepped)
	{
		int tile = map[p.y * mapSize.x + p.x];
		steppedFloorSprite->changeAnimation(0);
		if (tile == 4)
			steppedFloorSprite->changeAnimation(1);

		steppedFloorSprite->setPosition(glm::vec2(screenCoords.x + p[0]*tileSize, screenCoords.y + p[1] *tileSize));
		steppedFloorSprite->render();
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, bool bCrossPlattforms) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y < y1; y++)
	{
		int tile = map[y * mapSize.x + x];
		if (!bCrossPlattforms && tile != 0)
			return true;
		else if (tile == 6 || tile == 2 || tile == 4)
			return true;
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, bool bCrossPlattforms) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		int tile = map[y * mapSize.x + x];
		if (!bCrossPlattforms && tile != 0)
			return true;
		else if (tile == 6 || tile == 2 || tile == 4)
			return true;
	}
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool bCrossPlattforms) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];

		//if (tile == 3 || (!bCrossPlattforms && tile != 0))
		if ((!bCrossPlattforms && tile != 0) || tile == 6)
			if (*posY - tileSize * y < tileSize)
				return true;
	}
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		
		if (tile != 0)
		//if (tile != 0 && tile != 5 && tile != 6)
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	return false;
}

bool TileMap::allPlattformsStepped() const
{
	return nStepTiles <= 0;
}

bool TileMap::doorOpen()
{
	return false;
}

glm::vec2 TileMap::getRandomPosition(int radius)
{
	srand(time(NULL));

	vector<glm::ivec2> available;

	for (auto i = 0; i < plattforms.size(); ++i) {
		glm::vec2 pos = plattforms[i];

		if (!((pos.x > posPlayer.x / tileSize - radius && pos.x < (posPlayer.x + 32) / tileSize + radius)
			&& (pos.y > posPlayer.y / tileSize - radius && pos.y < (posPlayer.y + 32) / tileSize + radius))) {
			available.push_back(pos);
		}
	}

	// in pixels
	return (available[rand() % max(1,available.size())]) * tileSize; // ESTA MIERDA HA PETADO UNA VEZ
}

void TileMap::positionStepped(const glm::ivec2& pos, const glm::ivec2& size, int* posY)
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (*posY - tileSize * y + size.y <= 4)
		{
			int tile = map[y * mapSize.x + x];
			if (tile == 1 || tile == 2)
			{
				map[y * mapSize.x + x] = tile + 2;
				positionsStepped.push_back(glm::ivec2(x, y));
				--nStepTiles;
			}
			*posY = tileSize * y - size.y;
		}
	}

}

// Private functions

void TileMap::setHourglassTaken(bool b)
{
	hourglassTaken = b;
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(0.25f, 0.5f);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			//Skeleton
			else if (tile == 'S') {
				enemies.push_back(make_pair('S', glm::ivec2(i,j)));
				map[j * mapSize.x + i] = 0;
			}
			// Vampire
			else if (tile == 'V') {
				enemies.push_back(make_pair('V', glm::ivec2(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			// Reaper
			else if (tile == 'R') {
				enemies.push_back(make_pair('R', glm::ivec2(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			//Lava
			else if (tile == 'X') {
				enemies.push_back(make_pair('X', glm::ivec2(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			//Saw
			else if (tile == 'x') {
				enemies.push_back(make_pair('x', glm::ivec2(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			//Player
			else if (tile == 'P') {
				posPlayer = glm::ivec2(i, j);
				map[j * mapSize.x + i] = 0;
			}
			//Door
			else if (tile == 'D') {

				posDoor = glm::ivec2(i, j);

				map[j * mapSize.x + i] = 0;
			}
			else {
				//Steppable platform
				if (tile == '1' || tile == '2')
				{
					++nStepTiles;
					plattforms.push_back(glm::ivec2(i, j));
				}
				map[j * mapSize.x + i] = tile - int('0');
			}
		}
		fin.get(tile);

#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.25f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				//texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	cout << "tileTexSize: " << tileTexSize[0] << " " << tileTexSize[1] << endl;
	cout << "nTiles: " << nTiles << endl;
	cout << "tileSize: " << tileSize << endl;
	cout << "blockSize: " << blockSize << endl;
	cout << "mapSize: " << mapSize[0] << " " << mapSize[1] << endl;
	cout << "tileSheetSize: " << tilesheetSize[0] << " " << tilesheetSize[1] << endl;
	cout << "position: " << position.x << " " << position.y << endl;
}