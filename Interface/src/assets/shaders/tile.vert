#version 120

// The Tile's vertex data is initially stored normalized, so we stretch 
// and move it to the size and position of the Tile. We also translate
// each vertex so that the origin is at the top left corner of the screen,
// not the center.

// Uniform vector for the location of the Tile.
uniform vec2 location;

// Unifrom vector for the size of the Tile.
uniform vec2 size;

// The texture coordinate vector.
centroid varying vec2 texCoord;

// The main function of the vertex shader.
void main()
{
	// Copy the incoming vert data into a vec4 for easy manipulation.
	vec4 v = gl_Vertex;
	
	// scale the vertex by the size given.
	v.xy *= size;
	
	// Move the vertex to the location given.
	v.xy += location;
	
	// Center the Tile on its position for appearance when shifting size.
	v.xy -= size/2;
	
	// Multiply the vertex by 2 to make the screen space 1x1.
	v.xy *= 2.000;
	
	// Do the only scene transformation needed in our 2D environment--shiftting the 
	// origin from the center to the top left of the screen.
	v.xy-=1.000;
	
	v.y *= -1.000;
	
	// Copy our vertex data to the output variable.
	gl_Position = v;
	
	
	// Set up the texture coordinate vector so that it can go into the fragment shader.
	texCoord = vec2(gl_MultiTexCoord0);
}


