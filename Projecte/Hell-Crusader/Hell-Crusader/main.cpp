#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


//Remove console (only works in Visual Studio)
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#define TIME_PER_FRAME 1000.f / 60.f // Approx. 60 fps


static int prevTime;


// If a key is pressed this callback is called

static void keyboardDownCallback(unsigned char key, int x, int y)
{
	Game::instance().keyPressed(key);
}

// If a key is released this callback is called

static void keyboardUpCallback(unsigned char key, int x, int y)
{
	Game::instance().keyReleased(key);
}

// If a special key is pressed this callback is called

static void specialDownCallback(int key, int x, int y)
{
	Game::instance().specialKeyPressed(key);
}

// If a special key is released this callback is called

static void specialUpCallback(int key, int x, int y)
{
	Game::instance().specialKeyReleased(key);
}

// Same for changes in mouse cursor position

static void motionCallback(int x, int y)
{
	Game::instance().mouseMove(x, y);
}

// Same for mouse button presses or releases

static void mouseCallback(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
		Game::instance().mousePress(button);
	else if(state == GLUT_UP)
		Game::instance().mouseRelease(button);
}

#include <iostream>

static void drawCallback()
{
	Game::instance().render();
	glutSwapBuffers();
}

static void idleCallback()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - prevTime;
	
	if(deltaTime > TIME_PER_FRAME)
	{
		// Every time we enter here is equivalent to a game loop execution
		if(!Game::instance().update(deltaTime))
			exit(0);
		prevTime = currentTime;
		glutPostRedisplay();
	}
}

void positionWindow(int screenWidth, int screenHeight)
{
	RECT rect;
	HWND taskBar = FindWindow(TEXT("Shell_TrayWnd"), NULL);
	if (taskBar && GetWindowRect(taskBar, &rect))
	{
		int taskBarHeight = rect.bottom - rect.top;
		int windowWidth = screenWidth;
		int windowHeight = screenHeight - taskBarHeight;
		int windowPosX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
		int windowPosY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
		glutPositionWindow(windowPosX, windowPosY);
		glutReshapeWindow(windowWidth, windowHeight);
	}
	else
	{
		glutFullScreen();
	}
}


int main(int argc, char **argv)
{
	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	//int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	//int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
	//glutInitWindowPosition(100, 100);
	//positionWindow(screenWidth, screenHeight);
	//glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow(argv[0]);

	glutReshapeFunc(Game::reshapeCallback); //LA FUNCION ESTA EN LA CLASE GAME
	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	glutDisplayFunc(drawCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(keyboardDownCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialDownCallback);
	glutSpecialUpFunc(specialUpCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);

	// GLEW will take care of OpenGL extension functions
	glewExperimental = GL_TRUE;
	glewInit();
	
	// Game instance initialization
	Game::instance().init();
	prevTime = glutGet(GLUT_ELAPSED_TIME);
	// GLUT gains control of the application
	glutMainLoop();

	return 0;
}



