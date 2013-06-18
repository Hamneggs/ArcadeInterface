#include "Tile.h"


Tile::Tile(void)
{
	x = 0;
	y = 0;
	asx = 0;
	asy = 0;
	isx = 0;
	isy = 0;
	csx = 0;
	csy = 0;
	currentState = 0;
	intendedState = INACTIVE;
	stateDelta = 0.0;
	sizeXDelta = 0.0;
	sizeYDelta = 0.0;
	hasFrame = false;
	frameTexture = NULL;
	program = NULL;
	north = NULL;
	south = NULL;
	east = NULL;
	west = NULL;
	//name = NULL;
	//desc = NULL;
	//command = NULL;
	id = 0;
	locations = NULL;
	uvs = NULL;
}

/*
Provides this Tile with a reference to a configuration struct.
*/
bool Tile::init(UIConfig * c)
{
	this->c = c;
	return (this->c != NULL);
}

Tile::~Tile(void)
{
	destroyGPUHoldings();

	printf("Killing Tile Strings\n");
	if(name != NULL) free(name);
	if(desc != NULL) free(desc);
	if(command != NULL) free(command);
}

/*
Initializes the location of the Tile.
*/
bool Tile::initLocation(float x, float y)
{
	if(x!=NULL)this->x = x;
	if(y!=NULL)this->y = y;
	return true;
}

/*
Initializes the active and inactive sizes of the Tile.
*/
bool Tile::initSizes(float activeSizeX, float activeSizeY, float inactiveSizeX, float inactiveSizeY)
{
	if(activeSizeX!=NULL)this->asx = activeSizeX;
	if(activeSizeY!=NULL)this->asy = activeSizeY;
	if(inactiveSizeX!=NULL)this->isx = inactiveSizeX;
	if(inactiveSizeY!=NULL)this->isy = inactiveSizeY;

	csx = (asx * currentState) + (isx * (1-currentState));
	csy = (asy * currentState) + (isy * (1-currentState));
	return true;
}

/*
Initializes the textual info of the Tile.
*/
bool Tile::initInfo(char * name, char * description, char * command)
{
	if(name!=NULL)
	{
		this->name = (char*) malloc(sizeof(char)*(strlen(name)+1));
		strcpy(this->name, name);
	}
	if(description!=NULL)
	{
		this->desc = (char*) malloc(sizeof(char)*(strlen(description)+1));
		strcpy(this->desc, description);
	}
	if(command!=NULL)		
	{
		this->command = (char*) malloc(sizeof(char)*(strlen(command)+1));
		strcpy(this->command, command);
	}
	return true;
}

/*
Initializes the ever-important ID of the Tile.
*/
bool Tile::initID(unsigned int id)
{
	this->id = id;
	return true;
}

/*
Initializes the frame texture and the frame status of this Tile.
*/
bool Tile::initFrame(bool hasFrame, GLTexture * frameTexture)
{
	this->hasFrame = hasFrame;
	this->frameTexture = frameTexture;
	return true;
}

/*
Initializes the animation properties of this frame.
*/
bool Tile::initAnim(bool selectable)
{
	this->selectable = selectable;
	this->stateDelta = 1.00000f / c->anim_frames;
	this->sizeXDelta = (asx-isx) / c->anim_frames;
	this->sizeYDelta = (asy-isy) / c->anim_frames;
	return true;
}

/*
Loads the three screenshots from file using the relative to .exe paths given.
*/
bool Tile::initImages(char * activePath, char * inactivePath, char * bgPath)
{
	// Create a boolean to store the load status of the images.
	bool imageLoaded = false;

	// Create a boolean to store the success status of the Tile.
	bool success = true;

	if(activePath != NULL)
	{
		if(c->ext_tile_rep) printf("\nPATH: %s\n", activePath);
		if(c->ext_tile_rep) printf("\tloading image...");
		// Load the background image and display a warning if shit goes wrong.
		imageLoaded = active.loadTextureImage(activePath, false, GL_BGRA);
		if(!imageLoaded){
			printf("TILE ID: %d\nTILE NAME: %s\nERROR: Could not load active texture.\nPATH: %s", id, name, activePath);
			success = false;
		}

		if(c->ext_tile_rep) printf("\tsetting parameters...");
		// Set the filtering of the texture to be "nearest-neighbor" filtered when magnified to preserve native pixels,
		// and to be bilinearly filtered when minified to not look retarded.
		active.setFiltering(GL_NEAREST, GL_LINEAR);

		// Since this is the background texture we need it to scroll. The easiest way to do that is to enable
		// repeating texture wrap and just grow your UV coordinates beyond 1 or 0.
		active.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		active.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		active.setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		if(c->ext_tile_rep) printf("\tdone.");
	}

	if(inactivePath != NULL)
	{
		if(c->ext_tile_rep) printf("\nPATH: %s\n", inactivePath);
		if(c->ext_tile_rep) printf("\tloading image...");
		imageLoaded = inactive.loadTextureImage(inactivePath, false, GL_BGRA);
		if(!imageLoaded){
			printf("TILE ID: %d\nTILE NAME: %s\nERROR: Could not load inactive texture.\nPATH: %s", id, name, inactivePath);
			success = false;
		}
		if(c->ext_tile_rep) printf("\tsetting parameters...");
		inactive.setFiltering(GL_NEAREST, GL_LINEAR);
		inactive.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		inactive.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		inactive.setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		if(c->ext_tile_rep) printf("\tdone.");
	}

	if(bgPath != NULL)
	{
		if(c->ext_tile_rep) printf("\nPATH: %s\n", bgPath);
		if(c->ext_tile_rep) printf("\tloading image...");
		imageLoaded = bg.loadTextureImage(bgPath, false, GL_BGRA);
		if(!imageLoaded){
			printf("TILE ID: %d\nTILE NAME: %s\nERROR: Could not load background texture.\nPATH: %s", id, name, bgPath);
			success = false;
		}
		if(c->ext_tile_rep) printf("\tsetting parameters...");
		bg.setFiltering(GL_NEAREST, GL_LINEAR);
		bg.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		bg.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		bg.setSamplerParameter(GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
		if(c->ext_tile_rep) printf("\tdone.");
	}

	return success;
}

/*
Initializes the neighbor relationships.
*/
bool Tile::initNeighbors(Tile * north, Tile * south, Tile * east, Tile * west)
{
	if(north != NULL)	this->north = north;
	if(south != NULL)	this->south = south;
	if(east != NULL)	this->east = east;
	if(west != NULL)	this->west = west;
	return true;
}

/*
Initializes the holdings of the Tile in GPU memory by means of vaos.
*/
bool Tile::initGPUHoldings(ShaderProgram * program)
{
	this->program = program;
	initBuffers();
	return true;
}

/*
Destroys presence in GPU memory.
*/
bool Tile::destroyGPUHoldings(void)
{
	active.deleteTexture();
	inactive.deleteTexture();
	bg.deleteTexture();
	destroyBuffers();
	return true;
}

/*
Renders the tile to the screen.
*/
void Tile::render(void)
{
	// We must first perform all the physical animation duties.
	handleAnimation();
	
	// Begin using the shader.
	program->useProgram();

	// Bind all the textures to a specific slot for use within the shader.
    frameTexture->bindTexture(0);
	active.bindTexture(1);
	inactive.bindTexture(2);

	// Create an unsigned int to store where the uniform variables are located
	// within the compiled shader program.
	GLuint uniLoc;
	// Find those uniforms.
	uniLoc = glGetUniformLocation(program->getProgramID(), "location");
	glUniform2f(uniLoc, x, y);
	uniLoc = glGetUniformLocation(program->getProgramID(), "size");
	glUniform2f(uniLoc, csx, csy);
	uniLoc = glGetUniformLocation(program->getProgramID(), "frameTexture");
	glUniform1i(uniLoc, 0);
	uniLoc = glGetUniformLocation(program->getProgramID(), "hasFrame");
	glUniform1i(uniLoc, (int)hasFrame);
	uniLoc = glGetUniformLocation(program->getProgramID(), "activeTexture");
	glUniform1i(uniLoc, 1);
	uniLoc = glGetUniformLocation(program->getProgramID(), "inactiveTexture");
	glUniform1i(uniLoc, 2);
	uniLoc = glGetUniformLocation(program->getProgramID(), "state");
	glUniform1f(uniLoc, currentState);
	uniLoc = glGetUniformLocation(program->getProgramID(), "time");
	glUniform1i(uniLoc, glutGet(GLUT_ELAPSED_TIME));
	uniLoc = glGetUniformLocation(program->getProgramID(), "activeTime");
	glUniform1i(uniLoc, activeTime);

	// Since we are targeting OpenGL 2.0, we have to do some wonky per-vertex glBegin and glEnd shit.
	glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < 4; i++)
		{
			// Actually do the vertex.
			glTexCoord2f(uvs[(i*2)], uvs[(i*2)+1]);
			glVertex3f(locations[(i*3)], locations[(i*3)+1], locations[(i*3)+2]);
		}
	glEnd();
	
	// Stop using the shader.
	program->stopUsingProgram();
}

/*
Returns whether or not this Tile is selectable.
*/
bool Tile::isSelectable(void)
{
	return this->selectable;
}

/*
Returns the X location of the Tile.
*/
float Tile::getX(void)
{
	return this->x;
}

/*
Returns the Y location of the Tile.
*/
float Tile::getY(void)
{
	return this->y;
}

/*
Returns the neighbor of this Tile in the given direction.
*/
Tile * Tile::getNeighbor(Direction dir)
{
	switch(dir)
	{
	case NORTH:
		return north;
	case SOUTH:
		return south;
	case EAST:
		return east;
	case WEST:
		return west;
	default:
		return NULL;
	};
}

/*
sets the location of the Tile.
*/
void Tile::setLocation(float x, float y)
{
	this->x = x;
	this->y = y;
}

/*
Changes the location of the Tile.
*/
void Tile::changeLocation(float x, float y)
{
	this->x += x;
	this->y += y;
}

/*
Returns the width of the Tile when active.
*/
float Tile::getActiveW(void)
{
	return this->asx;
}

/*
Returns the height of the Tile when active.
*/
float Tile::getActiveH(void)
{
	return this->asy;
}

/*
Returns the width of the Tile when inactive.
*/
float Tile::getInactiveW(void)
{
	return this->isx;
}

/*
Returns the height of the Tile when inactive.
*/
float Tile::getInactiveH(void)
{
	return this->isy;
}
	
/*
Returns the state of the Tile.
*/
float Tile::getState(void)
{
	return this->currentState;
}

/*
Returns the ID of the Tile.
*/
unsigned int Tile::getID(void)
{
	return this->id;
}

/*
Returns the name of the game represented by the Tile.
*/
char * Tile::getName(void)
{
	return this->name;
}

/*
Returns the command of this Tile.
*/
char * Tile::getCommand(void)
{
	return this->command;
}

/*
Sets the state of the tile.
*/
void Tile::setState(State state)
{
	if(state == ACTIVE)activeTime = glutGet(GLUT_ELAPSED_TIME);
	this->intendedState = state;
}

/*
Executes the command of the Tile.
*/
int Tile::execCommand(void)
{
	if(intendedState == ACTIVE)
	{
		intendedState = EXECUTE;
	}
	else
	{
		return 0;
	}
	return 1000;
}

/*
Returns the BG texture of the Tile.
*/
GLTexture * Tile::getBGTexture(void)
{
	return &bg;
}



/*
Initializes the OpenGL vertex buffers.
*/
void Tile::initBuffers(void)
{	
	// Create an array to store the vertices.
	locations = (float *) malloc(sizeof(float)*12);

	// Create all the vertex locations.
	locations[0]  = 0.0f;
	locations[1]  = 1.0f;
	locations[2]  = 0.0f;

	locations[3]  = 0.0f;
	locations[4]  = 0.0f;
	locations[5]  = 0.0f;

	locations[6]  = 1.0f;
	locations[7]  = 1.0f;
	locations[8]  = 0.0f;

	locations[9]  = 1.0f;
	locations[10] = 0.0f;
	locations[11] = 0.0f;

	// Create an array to store the vertex UV coordinates.
	uvs = (float * ) malloc(sizeof(float)*8);

	// Create all the UV coordinates.
	uvs[0] = 0.0f;
	uvs[1] = 0.0f;

	uvs[2] = 0.0f;
	uvs[3] = 1.0f;

	uvs[4] = 1.0f;
	uvs[5] = 0.0f;

	uvs[6] = 1.0f;
	uvs[7] = 1.0f;
}

/*
Destroys the OpenGL holdings for this Tile.
*/
void Tile::destroyBuffers(void)
{
	printf("\nSHIT'S GOIN DOWN!!!");
	free(locations);
    free(uvs);
}

/*
Handles frame-by-frame animation.
*/
void Tile::handleAnimation(void)
{
	switch(intendedState)
	{
	/*case EXECUTE:
		if(currentState < 2.0f)
		{
			currentState += stateDelta;
		}
		break;*/
	case ACTIVE:
		if(currentState < 1.0f)
		{
			currentState += stateDelta;
			csx += sizeXDelta;
			csy += sizeYDelta;
		}
		break;
	case INACTIVE:
		if(currentState > 0.0f)
		{
			currentState -= stateDelta;
			csx -= sizeXDelta;
			csy -= sizeYDelta;			
		}
		break;
	};
}
