#include "Background.h"


Background::Background(void)
{
	grid = NULL;
	locations = NULL;
	uvs = NULL;
}


Background::~Background(void)
{
	destroyBuffers();
}

/*
Initializes the background by creating its GL holdings and populating a
required reference to a TileGrid. Also initializes the currentTime pointer.
*/
bool Background::init(TileGrid * grid, UIConfig * c)
{
	printf("\nInitializing background...");
	// Initialize the vertex and UV buffers.
	initBuffers();

	// Internalize the grid reference.
	this->grid = grid;

	// Load and compile the two shaders for use.
	vert.loadShader(c->bg_v, GL_VERTEX_SHADER, c->ext_shader_rep);
	frag.loadShader(c->bg_f, GL_FRAGMENT_SHADER, c->ext_shader_rep);

	// Create a shader program and link the two shaders into it.
	program.createProgram();
	program.attachShaderToProgram(&vert);
	program.attachShaderToProgram(&frag);
	program.linkProgram();
	return program.isLinked();
	
}

/*
Renders the background. Simple as that.
*/
void Background::render(void)
{
	// Start using the shader program.
	program.useProgram();

	// Bind the textures to texture units.
	grid->getCurrent()->getBGTexture()->bindTexture(1);
	grid->getPrevious()->getBGTexture()->bindTexture(0);

	// Create an unsigned int to store where the uniform variables are located
	// within the compiled shader program.
	GLuint uniLoc;

	// Populate the uniforms.
	uniLoc = glGetUniformLocation(program.getProgramID(), "previous");
	glUniform1i(uniLoc, 0);
	uniLoc = glGetUniformLocation(program.getProgramID(), "current");
	glUniform1i(uniLoc, 1);
	uniLoc = glGetUniformLocation(program.getProgramID(), "state");
	if(grid->getCurrent() != NULL)
	{
		glUniform1f(uniLoc, grid->getCurrent()->getState());
	}
	else
	{
		glUniform1f(uniLoc, 1.0);
	}
	uniLoc = glGetUniformLocation(program.getProgramID(), "time");
	glUniform1i(uniLoc, glutGet(GLUT_ELAPSED_TIME));

	// Since we are targeting OpenGL 2.0, we have to do some wonky per-vertex glBegin and glEnd shit.
	glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < 4; i++)
		{
			// Actually do the vertex.
			glTexCoord2f(uvs[(i*2)], uvs[(i*2)+1]);
			glVertex3f(locations[(i*3)], locations[(i*3)+1], locations[(i*3)+2]);
		}
	glEnd();

	// Stop using the shader program.
	program.stopUsingProgram();

}

/*
Initializes the OpenGL vertex buffers.
*/
void Background::initBuffers(void)
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
void Background::destroyBuffers(void)
{
	free(locations);
	free(uvs);
}
