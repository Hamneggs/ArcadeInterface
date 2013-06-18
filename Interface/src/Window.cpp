#include "Window.h"

/*
A reference to the Grid stack so that we always 
know which Grid in the stack to render.
*/
TileGrid * grid = NULL;

/*
A reference to the BG instance used throughout the application
so that we can access its render function within
the rendering function and render it.
*/
Background * bg = NULL;

/*
A pointer to the HUD overlay so that we may be able to draw it as well.
*/
Hud * hud = NULL;

/*
We store a refrence to the application's configuration struct so that
we have access to the window preferences whenever we modify the main
window.
*/
UIConfig * c = NULL;

/*
Timing variables so that we can calculate fps. 
*/
int previousTime = 0, currentTime = 0;

/*
A c-string to store the fps results so that it may be passed in as the window title.
*/
char titleString[12];

/*
The current fullscreen/windowed status of the application.
*/
bool fullscreen;

namespace window
{

	/*
	Initialize the window for the first time. This initializes both GLUT and GLEW,
	and also calls the createWindow() function.
	*/
	bool init(int argc, char * argv[], UIConfig * config)
	{
		// Store a reference to the config struct.
		c = config;

		// Initialize GLUT, and the GLUT window.
		glutInit(&argc, argv);
		createWindow();

		// Now that GLUT is initialized, we can initialize GLEW.
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			printf("Error: %s\n", glewGetErrorString(err));
			return false;
		}
		else
		{
			printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
			printf("X: %d, Y: %d \n", c->x_res, c->y_res);

		}

		return true;

	}

	/*
	Creates the actual window based on the window properties 
	set in the configuration struct.
	*/
	bool createWindow(void)
	{
		
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
			printf("X: %d, Y: %d \n", c->x_res, c->y_res);


		glutInitWindowSize(c->x_res, c->y_res);
		glutInitWindowPosition(c->win_x, c->win_y);

		
		// Create the actual window.
		glutCreateWindow(c->title);
		glutSetCursor(GLUT_CURSOR_NONE);
		fullscreen = c->fullscr;
		if(fullscreen) glutFullScreen();

		// Set a basic clear color for OpenGL.
		glClearColor(c->clr_r, c->clr_g, c->clr_b, 1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		

		// FUCKING ENABLE DEPTH TESTING.
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Enable texturing.
		glEnable(GL_TEXTURE_2D);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glViewport(0, 0, c->x_res, c->y_res);
		glutDisplayFunc(render);
		glutIdleFunc(render);
		glutWindowStatusFunc(visibilityFunc);
		return true;
	}

	/*
	The main rendering function of the application. 
	This function is called by GLUT every frame.
	*/
	void render(void)
	{
		if(glutGetWindow() == 0)
		{
			return;
		}
		updateFPS();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if(bg != NULL)
		{
			bg->render();
		}
		if(grid != NULL)
		{
			grid->render();
		}
		if(hud != NULL)
		{
			hud->render();
		}
		glutSwapBuffers();
	}

	/*
	Sets the Background instance.
	*/
	void setBG(Background * background)
	{
		bg = background;
	}

	void setGrid(TileGrid * newGrid)
	{
		grid = newGrid;
	}

	/*
	Sets the Hud instance that we need to render.
	*/
	void setHUD(Hud * newHud)
	{
		hud = newHud;
	}

	/*
	Calculates the time taken to render the previous frame and uses the calculation
	to calculate the fps. Afterward it puts it in the titlestring and gives it to GLUT
	for the window.
	*/
	void updateFPS(void)
	{
		if(!c->fullscr && glutGetWindow() != 0)
		{
			currentTime = glutGet(GLUT_ELAPSED_TIME);
			sprintf(titleString, "FPS: %2.2f", (1000/float(currentTime-previousTime)));
			glutSetWindowTitle(titleString);
			previousTime = currentTime;
		}
	}

	/*
	We can't start an application that requires exclusive mode with system() if our GLUT window
	already has exclusive mode. Thus we must wait and perform the system() call after
	we know that the window no longer has exclusive mode. This is the only way to do it.
	*/
	void visibilityFunc(int state)
	{
		if(state == GLUT_NOT_VISIBLE)
		{
			if(grid->getCurrent()->getState() > 1.0)
			{
				
				grid->getCurrent()->setState(ACTIVE);
				printf("\ncommand: %s\n", grid->getCurrent()->getCommand());
				system(grid->getCurrent()->getCommand());
				glutShowWindow();
				window::setFullScreen(c->fullscr);
			}
		}
	}

	/*
	handles fullscreen switching.
	*/
	void setFullScreen(bool full)
	{
		if(fullscreen && !full)
		{
			printf("X: %d, Y: %d \n", c->x_res, c->y_res);
			glutReshapeWindow(c->x_res, c->y_res);
			glutPositionWindow(c->win_x, c->win_y);
			fullscreen = false;
		}
		else if(!fullscreen && full) 
		{
			glutFullScreen();
			fullscreen = true;
		}
	}

	/*
	Returns the fullscreen status of the window.
	*/
	bool getFullScreen(void)
	{
		return fullscreen;
	}

}