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
	printf("Initializing grid...");
	this->c = c;
	layers = std::vector<std::vector<Tile *> *>();
	curLayer = 0;
	printf(" ...done\n");
	return true;
}

/*
Initializes the GL components of the grid.
*/
bool TileGrid::initGLComponents(void)
{
	printf("Initializing grid GL...");
	frameTexture.loadTextureImage(c->frame_path, false, GL_BGRA);
	frameTexture.setFiltering(GL_NEAREST, GL_NEAREST);
	tileVert.loadShader(c->tile_v, GL_VERTEX_SHADER, c->ext_shader_rep);
	tileFrag.loadShader(c->tile_f, GL_FRAGMENT_SHADER, c->ext_shader_rep);
	tileProgram.createProgram();
	tileProgram.attachShaderToProgram(&tileVert);
	tileProgram.attachShaderToProgram(&tileFrag);
	tileProgram.linkProgram();
	printf(" ...done\n");
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
		// Since it is first it must be made active, and all other Tiles must
		// be made inactive.
		#pragma omp parallel for
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
	}

	// If the Tile is not marked as being First, we just push it to the back 
	// of the current layer.
	else
	{
		layers[curLayer]->push_back(tile);
	}

	// If the current Tile is null, that means there isn't one yet, and that we need
	// to make one.
	if(currentTile == NULL)
	{
		// Get the first tile in the grid since we can assume that at this point
		// only one has been added and set the currentTile to point to it.
		currentTile = layers[curLayer]->at(0);
		
		// Here too we set the animation deltas to move the current Tile to the center of
		// the screen within anim_frames number of frames.
		animDeltaX = (.5f-currentTile->getX())/c->anim_frames;
		animDeltaY = (.5f-currentTile->getY())/c->anim_frames;

		// Since it is the current Tile we should make it active.
		currentTile->setState(ACTIVE);
	}

	// If there is no previous Tile we create one.
	if(previousTile == NULL)
	{
		previousTile = layers[curLayer]->at(0);
	}
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
		return;
	}

	// Play the movement sound.
	audio::playMove();

	// Store the direction for easy necessity testing when actually
	// performing the animation.
	curDir = dir;

	// Iterate the current Tile based on the direction chosen.
	if(currentTile->getNeighbor(dir) != NULL)
	{
		previousTile = currentTile;
		currentTile = currentTile->getNeighbor(dir);
	}

	// Update the states of the two most important Tiles.
	previousTile->setState(INACTIVE);
	currentTile->setState(ACTIVE);

	// Set the deltas so that we can move smoothly between Tiles.
	animDeltaX = -(currentTile->getX() - previousTile->getX())/c->anim_frames;
	animDeltaY = -(currentTile->getY() - previousTile->getY())/c->anim_frames;
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
	// If we are currently shifting Tiles we don't want to allow layer shifting.
	if(curDir!=NONE)return;

	// Decrement the current layer.
	curLayer --;

	// Wrap it at the edge case.
	if(curLayer < 0) curLayer = layers.size()-1;

	// If this layer has at least one Tile in it, we can shift the current Tile
	// to the first one on the new layer.
	if(layers[curLayer]->size()>0)
	{
		// Play the movement sound.
		audio::playMove();

		// Deactivate the previously current tile.
		currentTile->setState(INACTIVE);

		// Assign that Tile to be the previous Tile so we can have 
		// consistent background switching.
		previousTile = currentTile;

		// Turn off the previous Tile.
		previousTile->setState(INACTIVE);

		// Set the currentTile to the first tile of the new layer.
		currentTile = layers[curLayer]->at(0);

		// Reset the current Tile.
		currentTile->setState(ACTIVE);


		// Set up the animation deltas so that the screen snaps to the new current Tile.
		animDeltaX = (.5f-currentTile->getX())/c->anim_frames;
		animDeltaY = (.5f-currentTile->getY())/c->anim_frames;

		// Set the current direction to a none NONE value so that the animation handler does a job.
		curDir=NORTH;
	}

	// If there are no Tiles in this layer, we just set the current and previous tiles to NULL.
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

	// Again, if we are changing Tiles we don't allow layer shifting.
	if(curDir!=NONE)return;

	// Increment the current layer.
	curLayer ++;

	// Wrap the current layer at this edge case.
	curLayer%=layers.size();

	// If this layer has at least one Tile in it, we can shift the current Tile
	// to the first one on the new layer.
	if(layers[curLayer]->size()>0)
	{
		// Play the movement sound.
		audio::playMove();

		// Deactivate the previously current tile.
		currentTile->setState(INACTIVE);

		// Assign that Tile to be the previous Tile so we can have 
		// consistent background switching.
		previousTile = currentTile;

		// Turn off the previous Tile.
		previousTile->setState(INACTIVE);

		// Set the currentTile to the first tile of the new layer.
		currentTile = layers[curLayer]->at(0);

		// Reset the current Tile.
		currentTile->setState(ACTIVE);

		// Set up the animation deltas so that the screen snaps to the new current Tile.
		animDeltaX = (.5f-currentTile->getX())/c->anim_frames;
		animDeltaY = (.5f-currentTile->getY())/c->anim_frames;

		// Set the current direction to a none NONE value so that the animation handler does a job.
		curDir=NORTH;
	}

	// If there are no Tiles in this layer, we just set the current and previous tiles to NULL.
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
	// Range check the new value.
	curLayer = layer;
	curLayer%= layers.size();

	// If we can, we set the current Tile to the first one in the layer,
	// maintaining the previous one. We also set the animation constants.
	if(layers[curLayer]->size()>0)
	{
		currentTile->setState(INACTIVE);
		previousTile = currentTile;
		currentTile = layers[curLayer]->at(0);
		currentTile->setState(ACTIVE);
		previousTile->setState(INACTIVE);
		animDeltaX = (.5f-currentTile->getX())/c->anim_frames;
		animDeltaY = (.5f-currentTile->getY())/c->anim_frames;
		currentTile->setState(ACTIVE);
		curDir=NORTH;
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
	if(abs(currentTile->getX()-.5f) < abs(animDeltaX*1.1) )
	{
		animDeltaX = 0;
	}

	// We reset the Y movement factor when we don't need to move 
	// vertically anymore.
	if(abs(currentTile->getY()-.5f) < abs(animDeltaY*1.1) )
	{
		animDeltaY = 0;
	}

	// If we don't need to move any more, we reset the movement direction.
	if(abs(currentTile->getX()-.5f) < abs(animDeltaX*1.1) && 
		abs(currentTile->getY()-.5f) < abs(animDeltaY*1.1) )
	{
		curDir = NONE;
	}

	// If the animation deltas are super tiny we call it quits because
	// any movement that generates a delta that small is redundant and
	// possibly unwanted.
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
	int max = (int)layers[curLayer]->size();
	for(int i = 0; i < max; i++)
	{
		layers[curLayer]->at(i)->changeLocation(changeX, changeY);
	}
}
