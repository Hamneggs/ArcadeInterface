#pragma once
#include <glew.h>
#include <stdlib.h>
#include <gl\glut.h>
#include "Background.h"
#include "TileGrid.h"
#include "Hud.h"
#include "UIConfig.h"

namespace window
{
bool init(int argc, char * argv[], UIConfig * c);

bool createWindow(void);

void render(void);

void setGrid(TileGrid * grid);
void setBG(Background * background);
void setHUD(Hud * hud);

void setTimer(float * timer);
void updateFPS(void);
void visibilityFunc(int state);

void setFullScreen(bool full);
bool getFullScreen(void);
}
