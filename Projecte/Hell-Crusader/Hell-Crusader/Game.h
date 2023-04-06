#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"


//#define SCREEN_WIDTH 640
//#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 628
#define SCREEN_HEIGHT 500


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game& instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	static void reshapeCallback(int width, int height);
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	// Getters & Setters
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	void initShaders();
	void renderProjection();
	void updateMenu(int deltaTime);
	void updateScene(int deltaTime);
	void updateRatioWindowSize(int width, int height);
	void loadFirstStage();
	void loadNextStage();
	void optionSelected();

private:
	ShaderProgram texProgram;

	glm::mat4 projection;
	float sceneRatio = 1.28f;
	float menuRatio = 720.f/405.f;
	float ratioWindowSize;

	bool bPlay;                       // Continue to play game?
	Scene *scene;                      // Scene to render
	Menu menu;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	bool bMenuShowing;

	enum Option { Play, Instructions, Exit }; // There will be 4 saprites of the menu
														// everyone with one button selected
	map<int, Option> dictOptions;
	int option_nth;

	vector<string> stages;
	int stageIterator;
};


#endif // _GAME_INCLUDE


