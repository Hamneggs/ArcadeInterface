#pragma once
#include "TileGrid.h"
#include "UIConfig.h"

/*
This class handles the rendering of the background. It takes the state of both
the previous and current tiles and renders (as the background) the bg image 
of both based on their states. 
*/
class Background
{
public:
	Background(void);
	~Background(void);

	/*
	Initializes the background by creating its GL holdings and populating a
	required reference to a TileGrid.
	*/
	bool init(TileGrid * grid, UIConfig * c);

	/*
	Renders the background. Simple as that.
	*/
	void render(void);

private:

	/*
	The TileGrid that is used to get Tiles for their background images.
	*/
	TileGrid * grid;

	/*
	The array of vertex locations.
	*/
	float * locations;

	/*
	The array of vertex UV coordinates.
	*/
	float * uvs;

	/*
	The shader objects used in the background shader program.
	*/
	Shader vert, frag;

	/*
	The actual shader program that is used to draw the background.
	*/
	ShaderProgram program;

	/*
	Initializes the coordinate and UV buffers of the background.
	*/
	void initBuffers(void);

	/*
	Destroys the coordinate and UV buffers of the background.
	*/
	void destroyBuffers(void);

};

