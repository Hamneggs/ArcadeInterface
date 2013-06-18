#include "Keyboard.h"

namespace input
{
	std::vector<unsigned char> keysPressed;
	std::vector<int> specialKeysPressed;
	TileGrid * grid = NULL;
	Background * bg;
	Hud * hud = NULL;
	UIConfig * c;

	int timeExitPressed = 0;

	bool init(TileGrid * newGrid, Hud * newHUD, Background * newBG, UIConfig * newConfig)
	{
		grid = newGrid;
		hud = newHUD;
		bg = newBG;
		c = newConfig;

		glutKeyboardFunc(keyPressed);
		glutSpecialFunc(specialPressed);
		glutKeyboardUpFunc(keyReleased);
		glutSpecialUpFunc(specialReleased);
		return true;
	}

	bool addKey(unsigned char key)
	{
		for(unsigned int i = 0; i < keysPressed.size(); i++)
		{
			if(keysPressed[i]==key)
			{
				return false;
			}
		}
		keysPressed.push_back(key);
		return true;
	}

	bool addSpecialKey(int key)
	{
		for(unsigned int i = 0; i < specialKeysPressed.size(); i++)
		{
			if(specialKeysPressed[i]==key)
			{
				return false;
			}
		}
		specialKeysPressed.push_back(key);
		return true;
	}

	bool removeKey(unsigned char key)
	{
		for(unsigned int i = 0; i < keysPressed.size(); i++)
		{
			if(keysPressed[i]==key)
			{
				keysPressed.erase(keysPressed.begin()+i);
				return true;
			}
		}
		return false;
	}

	bool removeSpecialKey(unsigned int key)
	{
		for(unsigned int i = 0; i < specialKeysPressed.size(); i++)
		{
			if(specialKeysPressed[i]==key)
			{
				specialKeysPressed.erase(specialKeysPressed.begin()+i);
				return true;
			}
		}
		return false;
	}

	bool isKeyPressed(unsigned char key)
	{
		for(unsigned int i = 0; i < keysPressed.size(); i++)
		{
			if(keysPressed[i]==key)
			{
				return true;
			}
		}
		return false;
	}
	bool isSpecialKeyPressed(unsigned int key)
	{
		for(unsigned int i = 0; i < specialKeysPressed.size(); i++)
		{
			if(specialKeysPressed[i]==key)
			{
				return true;
			}
		}
		return false;
	}

	void keyPressed(unsigned char key, int x, int y)
	{
		bool entered = addKey(key);
		if(key == c->sdk_a || key == c->sdk_b)
		{
			if(entered == true)
			{
				if(isKeyPressed(c->sdk_a) && isKeyPressed(c->sdk_b))
				{
					timeExitPressed = glutGet(GLUT_ELAPSED_TIME);
				}
			}
			else
			{
				if(isKeyPressed(c->sdk_a) && isKeyPressed(c->sdk_b) && glutGet(GLUT_ELAPSED_TIME) - timeExitPressed > c->shutdown_time)
				{
					exit(0);
				}
				else if(isKeyPressed(c->sdk_a) && isKeyPressed(c->sdk_b) && glutGet(GLUT_ELAPSED_TIME) - timeExitPressed > c->shutdown_time/3)
				{
					if(hud != NULL)
					{
						hud->setWarning(true);
					}
				}
			}
		}
		else if(isKeyPressed(c->select))
		{
			// When we execute the command, GLUT misses the releaseKey event
			// of the select key. That means we have to deliberately remove it.
			removeKey(key);
			if(grid->getCurrent()->isSelectable()){
				window::setFullScreen(false);
				glutHideWindow();
				glutIconifyWindow();
				grid->getCurrent()->execCommand();
			}
		}
		else if(isKeyPressed(c->sdk_f))
		{
			exit(0);
		}
		else if(isKeyPressed(c->window))
		{
			window::setFullScreen(!window::getFullScreen());
		}
		else if(isKeyPressed(c->n_cfg))
		{
			grid->shiftForwardLayer();
		}
		else if(isKeyPressed(c->p_cfg))
		{
			grid->shiftBackLayer();
		}

	}

	void specialPressed(int key, int x, int y)
	{
		bool entry = addSpecialKey(key);
		if(grid != NULL)
		{
			if(key == GLUT_KEY_UP)
			{
				grid->changeTiles(NORTH);
			}
			else if(key == GLUT_KEY_DOWN)
			{
				grid->changeTiles(SOUTH);
			}
			else if(key == GLUT_KEY_LEFT)
			{
				grid->changeTiles(WEST);
			}
			else if(key == GLUT_KEY_RIGHT)
			{
				grid->changeTiles(EAST);
			}
		}
	}
	
	void keyReleased(unsigned char key, int x, int y)
	{
		bool removed = removeKey(key);
		hud->setWarning(false);
	}
	void specialReleased(int key, int x, int y)
	{
		bool removed = removeSpecialKey(key);
	}
}