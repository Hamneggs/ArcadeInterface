#pragma once
#include <vector>
#include "UIConfig.h"
#include "TileGrid.h"
#include "Hud.h"
#include <GL\glut.h>
#include "Window.h"


namespace input
{

	/*
	Initializes the data structures, application element references 
	and config data of the input code.
	*/
	bool init(TileGrid * newGrid, Hud * hud, Background * bg, UIConfig * c);

	/*
	Adds a key to the internal key-pressed stack.
	*/
	bool addKey(unsigned char key);

	/*
	Adds a special key to the internal special key-pressed stack.
	*/
	bool addSpecialKey(unsigned int key);

	/*
	Removes a key from the internal key-pressed stack.
	*/
	bool removeKey(unsigned char key);

	/*
	Removes a special key from the inernal special key-pressed stack.
	*/
	bool removeSpecialKey(unsigned int key);

	/*
	Returns whether or not a given key is in the pressed-key stack.
	*/
	bool isKeyPressed(unsigned char key);

	/*
	Returns whether or not a given special key is in the pressed-special key stack.
	*/
	bool isSpecialKeyPressed(unsigned int key);

	/*
	The function passed to GLUT as our keyPressed callback.
	*/
	void keyPressed(unsigned char key, int x, int y);

	/*
	The function passed to GLUT as our specialKeyPressed callback.
	*/
	void specialPressed(int key, int x, int y);

	/*
	The function passed to GLUT as our keyReleased callback.
	*/
	void keyReleased(unsigned char key, int x, int y);

	/*
	The function passed to GLUT as our specialKeyReleased callback.
	*/
	void specialReleased(int key, int x, int y);
};