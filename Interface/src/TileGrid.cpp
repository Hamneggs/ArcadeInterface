#include "TileGrid.h"


TileGrid::TileGrid(void)
{
	currentTile = NULL;
	previousTile  = NULL;
	animDeltaX = 0.0;
	animDeltaY = 0.0;
	curDir = SOUTH;
}


TileGrid::~TileGrid(void)
{
}

/*
Initializes the storage components of the grid.
*/
bool TileGrid::init(UIConfig * c)
{
	this->c = c;
	layers = std::vector<std::vector<Tile *> *>();
	curLayer = 0;
	return true;
}

/*
Initializes the GL components of the grid.
*/
bool TileGrid::initGLComponents(void)
{
	frameTexture.loadTextureImage(c->frame_path, false, GL_BGRA);
	frameTexture.setFiltering(GL_NEAREST, GL_NEAREST);
	tileVert.loadShader(c->tile_v, GL_VERTEX_SHADER, c->ext_shader_rep);
	tileFrag.loadShader(c->tile_f, GL_FRAGMENT_SHADER, c->ext_shader_rep);
	tileProgram.createProgram();
	tileProgram.attachShaderToProgram(&tileVert);
	tileProgram.attachShaderToProgram(&tileFrag);
	tileProgram.linkProgram();
	return tileProgram.isLinked();
}

/*
Destroys the GL components of the grid. This frees GPU memory.
*/
bool TileGrid::destroyGLComponents(void)
{
	frameTexture.deleteTexture();
	tileProgram.deleteProgram();
	tileVert.deleteShader();
	tileFrag.deleteShader();
	return true;
}

/*
Adds a Tile TileGrid::to the Grid.
*/
bool TileGrid::addTile(Tile *tile, bool first)
{
	// Make sure that we have a layer to add the Tile to.
	if(layers.size()<1)
	{
		printf("YOU MUST ADD A LAYER FIRST DIPWAD\n");
		return false;
	}

	// Make sure that the Tile doesn't already exist.
	for(unsigned int i = 0; i < layers[curLayer]->size(); i ++)
	{
		if(layers[curLayer]->at(i)->getID() == tile->getID())
		{
			printf("Could not add Tile. Duplicate ID.");
			return false;
		}
	}

	// If the tile is marked as being first, we added it to the beginning
	// of the Layer.
	if(first)
	{
		printf("FIRST BITCHES!!!\n");
		// Since it is first it must be made active, and all other Tiles must
		// be made inactive.
		for(unsigned int i = 0; i < layers[curLayer]->size(); i ++)
		{
			layers[curLayer]->at(i)->setState(INACTIVE);
		}
		

		// Add the Tile to the beginning of the layer.
		layers[curLayer]->insert(layers[curLayer]->begin(), tile);

		// Set the current tile to be this Tile, and preload the animation
		// deltas so that the grid is moved so the current Tile is centered
		// on the screen.
		currentTile = layers[curLayer]->at(0);
		currentTile->setState(ACTIVE);
		animDeltaX = (.5f-currentTile->getX())/c->anim_frames;
		animDeltaY = (.5f-currentTile->getY())/c->anim_frames;
		
		// Set the current direction to one that is not NONE so that the 
		// animation handler knows to move the Tiles.
		//curDir = SOUTH;
	}

	// If the Tile is not marked as being First, we just push it to the back 
	// of the current layer.
	else
	{
		printf("NOT FIRST!!!\n");
		layers[curLayer]->push_back(tile);
	}

	// If the current Tile is null, that means there isn't one yet, and that we need
	// to make one.
	if(currentTile == NULL)
	{
		printf("CURRENT TILE IS NULL\n");
		// Get the first tile in the grid since we can assume that at this point
		// only one has been added and set the currentTile to point to it.
		currentTile = layers[curLayer]->at(0);
		
		// Here too we set the animation deltas to move the current Tile to the center of
		// the screen within anim_frames number of frames.
		animDeltaX = (.5f-currentTile->getX())/c->anim_frames;
		animDeltaY = (.5f-currentTile->getY())/c->anim_frames;

		// Since it is the current Tile we should make it active.
		currentTile->setState(ACTIVE);

		// Set the current direction to a non-NONE value so that the animation
		// handler does its job.
		//curDir = SOUTH;
	}

	// If there is no previous Tile we create one.
	if(previousTile == NULL)
	{
		printf("PREVIOUS TILE IS NULL\n");
		previousTile = layers[curLayer]->at(0);
	}
	
	printf("\nP: %10s\nC:%10s\n", previousTile->getName(), currentTile->getName());
	return true;
}

/*
Returns a Tile TileGrid::based on its ID.
*/
Tile * TileGrid::getTile(unsigned int ID)
{
	for(unsigned int i = 0; i < layers[curLayer]->size(); i ++)
	{
		if(layers[curLayer]->at(i)->getID() == ID)
		{
			return layers[curLayer]->at(i);
		}
	}
	return NULL;
}

/*
Renders the Tile grid.
*/
void TileGrid::render(void)
{
	// Go through all the Tiles...
	for(unsigned int i = 0; i < layers[curLayer]->size(); i ++)
	{
		// ...And if it's NOT on screen...
		if(layers[curLayer]->at(i)->getX() < (0-layers[curLayer]->at(i)->getActiveW()) ||
			layers[curLayer]->at(i)->getX() > (1+layers[curLayer]->at(i)->getActiveW()) || 
			layers[curLayer]->at(i)->getY() < (0-layers[curLayer]->at(i)->getActiveH()) || 
			layers[curLayer]->at(i)->getY() > (1+layers[curLayer]->at(i)->getActiveH()))
		{
			// skip rendering it.
			continue;
		}
		// Otherwise, do render it.
		else
		{
			layers[curLayer]->at(i)->render();
		}
	}
	handleAnimation();
}

/*
Move the Grid in a specified direction by simply moving all
the tiles by the distance to the next Tile in the specified
direction. This also changes the currently active tile.
*/
void TileGrid::changeTiles(Direction dir)
{
	// If we don't have  a current Tile yet, we can't move from it.
	if(currentTile == NULL || previousTile == NULL || curDir != NONE)
	{
		printf("\nTILES NOT CHANGED");
		printf("\nCURRENT TILE NULL? %d", int(currentTile == NULL));
		printf("\nPREVIOUS TILE NULL? %d", int(previousTile == NULL));
		printf("\nCUR DIR IS NOT NONE? %d", int(curDir != NONE));
		
		printf("\nP: %10s\nC:%10s\n", previousTile->getName(), currentTile->getName());
		return;
	}

	// Store the direction for easy necessity testing when actually
	// performing the animation.
	curDir = dir;

	// Iterate the current Tile based on the direction chosen.
	if(currentTile->getNeighbor(dir) != NULL)
	{
		previousTile = currentTile;
		currentTile = currentTile->getNeighbor(dir);
	}
	//printf("\nCURRENT TILE ID: %d\nPREVIOUS TILE ID: %d", currentTile->getID(), previousTile->getID());

	// Update the states of the Tiles.
	for(unsigned int i = 0; i < layers[curLayer]->size(); i++)
	{
		layers[curLayer]->at(i)->setState(INACTIVE);
	}
	currentTile->setState(ACTIVE);

	// Set the deltas so that we can move smoothly between Tiles.
	animDeltaX = -(currentTile->getX() - previousTile->getX())/c->anim_frames;
	animDeltaY = -(currentTile->getY() - previousTile->getY())/c->anim_frames;
	printf("\nP: %10s\nC:%10s\n", previousTile->getName(), currentTile->getName());
}

/*
Triggers the command of the current active Tile, 
if there is one.
*/
int TileGrid::triggerCommand(void)
{
	if(currentTile != NULL)
	{
		return currentTile->execCommand();
	}
	else return 9001;
}

/*
Returns the current Tile.
*/
Tile * TileGrid::getCurrent(void)
{
	return currentTile;
}

/*
Returns the previous Tile.
*/
Tile * TileGrid::getPrevious(void)
{
	return previousTile;
}

/*
Returns the GLTexture of the Frame.
*/
GLTexture * TileGrid::getFrameTexture(void)
{
	return &frameTexture;
}

/*
Returns a pointer to the actual vector of Tiles.
*/
std::vector<Tile*> * TileGrid::getTiles(void)
{
	return layers[curLayer];
}

/*
Returns a pointer to the Tile shader.
*/
ShaderProgram * TileGrid::getTileProgram(void)
{
	return &tileProgram;
}

/*
Shifts back a layer.
*/
void TileGrid::shiftBackLayer(void)
{
	if(curDir!=NONE)return;
	curLayer --;
	if(curLayer < 0) curLayer = layers.size()-1;
	if(layers[curLayer]->size()>0)
	{
		currentTile->setState(INACTIVE);
		previousTile = currentTile;
		currentTile = layers[curLayer]->at(0);
		previousTile = layers[curLayer]->at(0);
		currentTile->setState(INACTIVE);
		currentTile->setState(ACTIVE);
		animDeltaX = (.5f-currentTile->getX())/c->anim_frames;
		animDeltaY = (.5f-currentTile->getY())/c->anim_frames;
		// Update the states of the Tiles.
		for(unsigned int i = 0; i < layers[curLayer]->size(); i++)
		{
			layers[curLayer]->at(i)->setState(INACTIVE);
		}
		currentTile->setState(ACTIVE);
		curDir=NORTH;
		printf("Layer: %d\nP: %10s\nC:%10s\n", curLayer, previousTile->getName(), currentTile->getName());
	}
	else
	{
		currentTile = NULL;
		previousTile = NULL;
	}
}

/*
Shifts forward a layer.
*/
void TileGrid::shiftForwardLayer(void)
{
	if(curDir!=NONE)return;
	curLayer ++;
	curLayer%=layers.size();
	if(layers[curLayer]->size()>0)
	{
		currentTile->setState(INACTIVE);
		previousTile = currentTile;
		currentTile = layers[curLayer]->at(0);
		previousTile = layers[curLayer]->at(0);
		currentTile->setState(INACTIVE);
		currentTile->setState(ACTIVE);
		animDeltaX = (.5f-currentTile->getX())/c->anim_frames;
		animDeltaY = (.5f-currentTile->getY())/c->anim_frames;
		// Update the states of the Tiles.
		for(unsigned int i = 0; i < layers[curLayer]->size(); i++)
		{
			layers[curLayer]->at(i)->setState(INACTIVE);
		}
		currentTile->setState(ACTIVE);
		curDir = NORTH;
		printf("Layer: %d\nP: %10s\nC:%10s\n", curLayer, previousTile->getName(), currentTile->getName());

	}
	else
	{
		currentTile = NULL;
		previousTile = NULL;
	}
}

/*
Shifts to a particular layer.
*/
void TileGrid::setLayer(unsigned int layer)
{
	animDeltaX = 0;
	animDeltaY = 0;
	curLayer = layer;
	curLayer%= layers.size();
	if(layers[curLayer]->size()>0)
	{
	
		//if(curDir!=NONE)return;
		
		
		currentTile->setState(INACTIVE);
		previousTile->setState(INACTIVE);
		previousTile = layers[curLayer]->at(0);
		currentTile = layers[curLayer]->at(0);
		currentTile->setState(INACTIVE);
		currentTile->setState(ACTIVE);
		animDeltaX = (.5-currentTile->getX())/c->anim_frames;
		animDeltaY = (.5-currentTile->getY())/c->anim_frames;
		// Update the states of the Tiles.
		for(unsigned int i = 0; i < layers[curLayer]->size(); i++)
		{
			layers[curLayer]->at(i)->setState(INACTIVE);
		}
		currentTile->setState(ACTIVE);
		curDir=NORTH;
		printf("Layer: %d\nP: %10s\nC:%10s\n", curLayer, previousTile->getName(), currentTile->getName());
	}
	else
	{
		currentTile = NULL;
		previousTile = NULL;
	}
}

/*
Adds a Layer.
*/
void TileGrid::addLayer(void)
{
	layers.push_back(new std::vector<Tile*>());
}

/*
Returns the number of layers.
*/
unsigned int TileGrid::getNumLayers(void)
{
	return layers.size();
}

/*
Returns the number of the current layer.
*/
unsigned int TileGrid::getCurrentLayer(void)
{
	return curLayer;
}

/*
Handles the animation of the Grid.
*/
void TileGrid::handleAnimation(void)
{
	
	// If the current direction being moved is in fact
	// a direction, we test to see which way we need to go.
	if(curDir != NONE && currentTile != NULL && previousTile != NULL)
	{
		// Make a single move call rather than several.
		changeTilesLocation(animDeltaX, animDeltaY);
	}

	// We reset the X movement factor when we don't need to move 
	// horizontally anymore.
	if(abs(currentTile->getX()-.5) < abs(animDeltaX*1.1) )
	{
		animDeltaX = 0;
	}

	// We reset the Y movement factor when we don't need to move 
	// vertically anymore.
	if(abs(currentTile->getY()-.5) < abs(animDeltaY*1.1) )
	{
		animDeltaY = 0;
	}

	// If we don't need to move any more, we reset the movement direction.
	if(abs(currentTile->getX()-.5) < abs(animDeltaX*1.1) && 
		abs(currentTile->getY()-.5) < abs(animDeltaY*1.1) )
	{
		curDir = NONE;
	}

	if(abs(animDeltaX)<.005 && abs(animDeltaY)<.005)
	{
		curDir = NONE;
	}
	
}

/*
Moves all tiles by a specific amount.
*/
void TileGrid::changeTilesLocation(float changeX, float changeY)
{
	for(unsigned int i = 0; i < layers[curLayer]->size(); i++)
	{
		layers[curLayer]->at(i)->changeLocation(changeX, changeY);
	}
}
