#include "Hud.h"


Hud::Hud(void)
{
}


Hud::~Hud(void)
{
	destroyBuffers();
}

/*
Initializes the HUD's UV and location buffers, as well as creates its shaders.
*/
bool Hud::init(float x, float y, float sx, float sy, UIConfig * c)
{
	printf("\nInitializing hud...");
	// Internalize all parameters.
	this->x = x;
	this->y = y;
	this->sx = sx;
	this->sy = sy;

	// Initialize the vertex and UV buffers.
	initBuffers();

	// Initialize the isWarning flag to be false.
	isWarning = false;

	// Load the vertex and fragment shaders.
	vert.loadShader(c->hud_v, GL_VERTEX_SHADER, c->ext_shader_rep);
	frag.loadShader(c->hud_f, GL_FRAGMENT_SHADER, c->ext_shader_rep);

	// Create a shader program and link the two shaders into it.
	program.createProgram();
	program.attachShaderToProgram(&vert);
	program.attachShaderToProgram(&frag);
	program.linkProgram();

	// Load the Overlay image.
	bool imageLoaded = false;
	if(c->ovr_path != NULL)
	{
		if(c->ext_tile_rep) printf("\nPATH: %s\n", c->ovr_path);
		if(c->ext_tile_rep) printf("\tloading image...");
		imageLoaded = hudTexture.loadTextureImage(c->ovr_path, false, GL_BGRA);
		if(!imageLoaded){
			printf("HUD TEXTURE: %s\nERROR: Could not load HUD texture.\n", c->ovr_path);
		}
		if(c->ext_tile_rep) printf("\tsetting parameters...");
		hudTexture.setFiltering(GL_LINEAR, GL_LINEAR);
		hudTexture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		hudTexture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		hudTexture.setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	// Load the exit warning image.
	if(c->exit_path != NULL)
	{
		if(c->ext_tile_rep) printf("\nPATH: %s\n", c->exit_path);
		if(c->ext_tile_rep) printf("\tloading image...");
		imageLoaded = warning.loadTextureImage(c->exit_path, false, GL_BGRA);
		if(!imageLoaded){
			printf("BACKGROUND ERROR TEXTURE: %s\nERROR: Could not load BG WARNING texture.\n");
		}
		if(c->ext_tile_rep) printf("\tsetting parameters...");
		warning.setFiltering(GL_NEAREST, GL_LINEAR);
		warning.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		warning.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		warning.setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	// Load the introduction image.
	if(c->intro_path != NULL)
	{
		if(c->ext_tile_rep) printf("\nPATH: %s\n", c->intro_path);
		if(c->ext_tile_rep) printf("\tloading image...");
		imageLoaded = intro.loadTextureImage(c->intro_path, false, GL_BGRA);
		if(!imageLoaded){
			printf("BACKGROUND ERROR TEXTURE: %s\nERROR: Could not load INTRO OVERLAY texture.\n");
		}
		if(c->ext_tile_rep) printf("\tsetting parameters...");
		intro.setFiltering(GL_NEAREST, GL_LINEAR);
		intro.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		intro.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		intro.setSamplerParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	else printf("\n\n\n OH SHIT INTRO NOT LOADED AH FUCK\n\n\n");

	return program.isLinked() && imageLoaded;
}

/*
Renders the HUD.
*/
void Hud::render(void)
{
	program.useProgram();
	
	// If the warning flag is set we pass in the exit warning rather than the overlay.
	if(isWarning) warning.bindTexture(0);
	else hudTexture.bindTexture(0);
	
	// Bind the intro to an independent texture unit.
	intro.bindTexture(1);

	// Garner the location of the uniforms.
	GLuint uniLoc;
	uniLoc = glGetUniformLocation(program.getProgramID(), "location");
	glUniform2f(uniLoc, x, y);
	uniLoc = glGetUniformLocation(program.getProgramID(), "size");
	glUniform2f(uniLoc, sx, sy);
	uniLoc = glGetUniformLocation(program.getProgramID(), "hudTexture");
	glUniform1i(uniLoc, 0);
	uniLoc = glGetUniformLocation(program.getProgramID(), "introTexture");
	glUniform1i(uniLoc, 1);
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
Sets the HUD shutdown warning status.
*/
void Hud::setWarning(bool warn)
{
	isWarning = warn;
}

/*
Initializes the coordinate and UV buffers of the HUD.
*/
void Hud::initBuffers(void)
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
Destroys the coordinate and UV buffers of the HUD.
*/
void Hud::destroyBuffers(void)
{
	free(locations);
	free(uvs);
}