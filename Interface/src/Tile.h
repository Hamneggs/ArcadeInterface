#pragma once
#include "GLTexture.h"
#include "Shaders.h"
#include "const.h"
#include "Tile.h"
#include "UIConfig.h"

/*
This enumuration details the various states a Tile can be in.
EXECUTE means a Tile has been marked to have its command executed.
ACTIVE means that the Tile is the current, in-focus Tile.
INACTIVE means that the Tile is not the current one.
*/
enum State
{
	EXECUTE = 2,
	ACTIVE = 1,
	INACTIVE = 0
};

/*
This enum is used to specify which neighbor binding point to access.
*/
enum Direction
{
	NORTH = 1,
	SOUTH = 2,
	EAST = 4,
	WEST = 8,
	NONE = 16
};

class Tile
{
public:

	/*
	Constructs a new Tile. This initializes all primative variables to a
	default state.
	*/
	Tile(void);

	/*
	Destructs the the Tile, clearing itself from GPU memory.
	*/
	~Tile(void);

	/*
	Provides this Tile with a reference to a configuration struct.
	*/
	bool init(UIConfig * c);

	/*
	Initializes the location of the Tile.
	*/
	bool initLocation(float x, float y);

	/*
	Initializes the active and inactive sizes of the Tile.
	*/
	bool initSizes(float activeSizeX, float activeSizeY, float inactiveSizeX, float inactiveSizeY);

	/*
	Initializes the textual info of the Tile.
	*/
	bool initInfo(char * name, char * description, char * command);

	/*
	Initializes the ever-important ID of the Tile.
	*/
	bool initID(unsigned int id);

	/*
	Initializes the frame texture and the frame status of this Tile.
	*/
	bool initFrame(bool hasFrame, GLTexture * frameTexture);

	/*
	Initializes the animation properties of this frame.
	*/
	bool initAnim(bool selectable);

	/*
	Loads the three screenshots from file using the relative to .exe paths given.
	*/
	bool initImages(char * activePath, char * inactivePath, char * bgPath);

	/*
	Initializes the neighbor relationships.
	*/
	bool initNeighbors(Tile * north, Tile * south, Tile * east, Tile * west);

	/*
	Initializes the holdings of the Tile in GPU memory by means of vaos.
	*/
	bool initGPUHoldings(ShaderProgram * program);

	/*
	Returns whether or not this Tile is selectable.
	*/
	bool isSelectable(void);

	/*
	Destroys presence in GPU memory.
	*/
	bool destroyGPUHoldings(void);

	/*
	Renders the Tile to the screen.
	*/
	void render(void);

	/*
	Returns the X location of the Tile.
	*/
	float getX(void);

	/*
	Returns the Y location of the Tile.
	*/
	float getY(void);

	/*
	Returns the neighbor of this Tile in the given direction.
	*/
	Tile * getNeighbor(Direction dir);

	/*
	sets the location of the Tile.
	*/
	void setLocation(float x, float y);

	/*
	Changes the location of the Tile.
	*/
	void changeLocation(float x, float y);

	/*
	Returns the width of the Tile when active.
	*/
	float getActiveW(void);

	/*
	Returns the height of the Tile when active.
	*/
	float getActiveH(void);

	/*
	Returns the width of the Tile when inactive.
	*/
	float getInactiveW(void);

	/*
	Returns the height of the Tile when inactive.
	*/
	float getInactiveH(void);
	
	/*
	Returns the state of the Tile.
	*/
	float getState(void);

	/*
	Returns the ID of the Tile.
	*/
	unsigned int getID(void);

	/*
	Returns the name of the game represented by the Tile.
	*/
	char * getName(void);

	/*
	Returns the command of this Tile.
	*/
	char * getCommand(void);

	/*
	Sets the state of the Tile.
	*/
	void setState(State state);

	/*
	Executes the command of the Tile.
	*/
	int execCommand(void);

	/*
	Returns the BG texture of the Tile.
	*/
	GLTexture * getBGTexture(void);

private:

	/*
	Initializes the coordinate and UV buffers of this Tile.
	*/
	void initBuffers(void);

	/*
	Destroys the coordinate and UV buffers of this Tile.
	*/
	void destroyBuffers(void);

	/*
	Handles frame-by-frame animation.
	*/
	void handleAnimation(void);

	/*
	The location of this Tile.
	*/
	float x, y;

	/*
	The active size of this Tile.
	*/
	float asx, asy;

	/*
	The inactve size of this Tile.
	*/
	float isx, isy;

	/*
	The current size of this Tile.
	*/
	float csx, csy;

	/*
	The delta of the size, used during animation.
	*/
	float sizeXDelta, sizeYDelta;

	/*
	The time that the Tile is set to active.
	*/
	int activeTime;

	/*
	The array of vertex locations.
	*/
	float * locations;

	/*
	The array of vertex UV coordinates.
	*/
	float * uvs;

	/*
	We store the current active/inactive state as a float so that we don't 
	have to lerp betwern therm.
	*/
	float currentState;

	/*
	The intended state is the one that the Tile is told to go to. 
	*/
	State intendedState;

	/*
	The per-frame delta of the state.
	*/
	float stateDelta;

	/*
	Whether or not this Tile uses a frame.
	*/
	bool hasFrame;

	/*
	Whether or not this Tile is selectable.
	*/
	bool selectable;

	/*
	The GLTexture that is this Tile's frame.
	*/
	GLTexture * frameTexture;

	/*
	The GLTexture that stores the active screenshot of this Tile.
	*/
	GLTexture active;

	/*
	The GLTexture that stores the inactive screenshot of this Tile.
	*/
	GLTexture inactive;

	/*
	The GLTexture that stores the background screenshot of this Tile.
	*/
	GLTexture bg;

	/*
	The Tile's vertex program.
	*/
	ShaderProgram * program;

	/*
	The neighbor Tiles of this Tile.
	*/
	Tile * north;
	Tile * south;
	Tile * east;
	Tile * west;

	/*
	The name of this Tile's game, and hence the name of the Tile.
	*/
	char * name;

	/*
	The description of this Tile's game.
	*/
	char * desc;

	/*
	The command that is run by selecting this Tile.
	*/
	char * command;

	/*
	This Tile's ID.
	*/
	unsigned int id;

	/*
	A reference to a config struct for animation and initialization.
	*/
	UIConfig * c;
};