#pragma once
#include <vector>
#include "Tile.h"
#include "const.h"
#include "UIConfig.h"

/*
This class represents a grid of Tiles (obviously.) This class tracks the
location and state of all Tiles and maintains a Tile reference that continuously
points to the current Tile the user has highlighted. This class is also
the avenue by which the Tiles are cycled through.

The TileGrid also supports multiple layers of Tiles, meaning that there can 
be multiple grids to cycle through. This can be exploited for organizational variety.
*/
class TileGrid
{
public:
	TileGrid(void);
	~TileGrid(void);

	/*
	Initializes the storage components of the grid.
	*/
	bool init(UIConfig * c);

	/*
	Initializes the GL components of the grid.
	*/
	bool initGLComponents(void);

	/*
	Destroys the GL components of the grid. This frees GPU memory.
	*/
	bool destroyGLComponents(void);

	/*
	Adds a Tile to the Grid.
	*/
	bool addTile(Tile * tile, bool first);

	/*
	Returns a Tile based on its ID.
	*/
	Tile * getTile(unsigned int ID);

	/*
	Renders the Tile grid.
	*/
	void render(void);

	/*
	Move the Grid in a specified direction by simply moving all
	the tiles by the distance to the next Tile in the specified
	direction. This also changes the currently active tile.
	*/
	void changeTiles(Direction dir);

	/*
	Triggers the command of the current active Tile, 
	if there is one.
	*/
	int triggerCommand(void);

	/*
	Returns the current Tile.
	*/
	Tile * getCurrent(void);

	/*
	Returns the previous Tile.
	*/
	Tile * getPrevious(void);

	/*
	Returns the GLTexture of the Frame.
	*/
	GLTexture * getFrameTexture(void);

	/*
	Returns a pointer to the actual vector of Tiles.
	*/
	std::vector<Tile *> * getTiles(void);

	/*
	Returns a pointer to the Tile shader.
	*/
	ShaderProgram * getTileProgram(void);

	/*
	Shifts back a layer.
	*/
	void shiftBackLayer(void);

	/*
	Shifts forward a layer.
	*/
	void shiftForwardLayer(void);

	/*
	Shifts to a particular layer.
	*/
	void setLayer(unsigned int layer);

	/*
	Adds a layer.
	*/
	void addLayer(void);

	/*
	Returns the number of layers.
	*/
	unsigned int getNumLayers(void);

	/*
	Returns the number of the current layer.
	*/
	unsigned int getCurrentLayer(void);


private:
	/*
	The vector that stores all the Tiles in the grid.
	*/
	std::vector<std::vector<Tile * > *> layers;

	/*
	The texture that is passed to each Tile by reference as
	the frame texture.
	*/
	GLTexture frameTexture;

	/*
	The Tile shader program and its components.
	*/
	Shader tileVert;
	Shader tileFrag;
	ShaderProgram tileProgram;

	/*
	The current Tile.
	*/
	Tile * currentTile;

	/*
	The previous Tile.
	*/
	Tile * previousTile;

	/*
	The delta of movement of the Tiles.
	*/
	float animDeltaX;
	float animDeltaY;

	/*
	The ID of this Grid.
	*/
	unsigned int id;

	/*
	The current movement direction of the Tile Grid.
	*/
	Direction curDir;

	/*
	A reference to the config struct for initialization and rendering.
	*/
	UIConfig * c;

	/*
	The current Tile layer.
	*/
	int curLayer;

	/*
	Handles the animation of the Grid.
	*/
	void handleAnimation(void);

	/*
	Moves all tiles by a specific amount.
	*/
	void changeTilesLocation(float changeX, float changeY);
};

