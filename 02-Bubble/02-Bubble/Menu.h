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

	void init();
	void update(int deltaTime);
	void render();
	void updateRatioWindowSize(int width, int height);

	// Gettters & Setters
	int getCurrentTime() { return currentTime; };

private:
	void initShaders();
	void updateTime(int deltatime);

private:
	ShaderProgram texProgram;

	glm::mat4 projection;
	float ratioWindowSize;

	int currentTime;
};


#endif // _SCENE_INCLUDE

