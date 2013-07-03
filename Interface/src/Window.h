#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED
#include <glew.h>
#include <stdlib.h>
#include <gl\glut.h>
#include "Background.h"
#include "TileGrid.h"
#include "Hud.h"
#include "UIConfig.h"
#include "SFX.h"

namespace window
{

/*
Initializes the various data structures used to handle user input.
*/
bool init(int argc, char * argv[], UIConfig * c);

/*
Creates a new rendering window. We need this because we need to blow
the window to oblivion each time we select a game so the emulator can
have exclusive mode.
*/
bool createWindow(void);

/*
The function that we supply to glut as our rendering callback. This
draws all the components of the application to the screen.
*/
void render(void);

/*
Assigns a TileGrid instance to rendering patrol.
*/
void setGrid(TileGrid * grid);

/*
Assigns a Background instance to be PLO.
*/
void setBG(Background * background);

/*
Assigns a Hud instance to be on watch tonight.
*/
void setHUD(Hud * hud);

/*
Calculates the time taken to render the previous frame and uses the calculation
to calculate the fps. Afterward it puts it in the titlestring and gives it to GLUT
for the window.
*/
void updateFPS(void);


/*
We can't start an application that requires exclusive mode with system() if our GLUT window
already has exclusive mode. Thus we must wait and perform the system() call after
we know that the window no longer has exclusive mode. This is the only way to do it.
*/
void visibilityFunc(int state);

/*
handles fullscreen switching.
*/
void setFullScreen(bool full);

/*
Returns whether or not the application is currently running fullscreen/exclusive mode.
*/
bool getFullScreen(void);
}
#endif
