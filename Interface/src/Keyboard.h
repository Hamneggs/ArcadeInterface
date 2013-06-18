#pragma once
#include <vector>
#include "UIConfig.h"
#include "TileGrid.h"
#include "Hud.h"
#include <GL\glut.h>
#include "Window.h"

namespace input
{
	bool init(TileGrid * newGrid, Hud * hud, Background * bg, UIConfig * c);
	bool addKey(unsigned char key);
	bool addSpecialKey(unsigned int key);
	bool removeKey(unsigned char key);
	bool removeSpecialKey(unsigned int key);
	bool isKeyPressed(unsigned char key);
	bool isSpecialKeyPressed(unsigned int key);
	void keyPressed(unsigned char key, int x, int y);
	void specialPressed(int key, int x, int y);
	void keyReleased(unsigned char key, int x, int y);
	void specialReleased(int key, int x, int y);
};