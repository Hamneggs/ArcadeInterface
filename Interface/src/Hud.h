#pragma once
#include "Shaders.h"
#include "GLTexture.h"
#include "const.h"
#include "UIConfig.h"

class Hud
{
public:
	Hud(void);
	~Hud(void);

	/*
	Initializes the HUD's UV and location buffers, as well as creates its shaders.
	*/
	bool init(float x, float y, float sx, float sy, UIConfig * c);

	/*
	Renders the HUD.
	*/
	void render(void);
	
	/*
	Toggles the warning display.
	*/
	void setWarning(bool warn);

private:

	/*
	The X and Y location of the HUD.
	*/
	float x, y;

	/*
	The size of the HUD.
	*/
	float sx, sy;

	/*
	The UV coordinate array.
	*/
	float * uvs;

	/*
	The location data array.
	*/
	float * locations;	

	/*
	The GLTexture used as the exit warning.
	*/
	GLTexture warning;

	/*
	The intro texture.
	*/
	GLTexture intro;

	/*
	The warning flag.
	*/
	bool isWarning;

	/*
	The shader objects used in the HUD shader program.
	*/
	Shader vert, frag;

	/*
	The shader program used to render the HUD.
	*/
	ShaderProgram program;

	/*
	The HUD texture.
	*/
	GLTexture hudTexture;

	/*
	Initializes the coordinate and UV buffers of the HUD.
	*/
	void initBuffers(void);

	/*
	Destroys the coordinate and UV buffers of the HUD.
	*/
	void destroyBuffers(void);
};

