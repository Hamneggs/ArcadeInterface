#version 120

// The BG has a comparatively simple vertex shader. Since the quad that the 
// background is rendered on is static, we don't need movement or size uniforms.
// we just make sure that the vertices are stretched to the four corners of the screen.

// The texture coordinate vector.
varying vec2 texCoord;

// The main function of the vertex shader.
void main()
{
	// Copy the incoming vert data into a vec4 for easy manipulation.
	vec4 v = gl_Vertex;
	
	// Multiply the vertex by 2 to make the screen space 1x1.
	v.xy *= 2.000;
	
	// Subtract 1 to align the vertex with the edges of the screen.
	v.xy -= 1.000;
	
	// Copy our vertex data to the output variable.
	gl_Position = v;
	
	// Set up the texture coordinate vector so that it can go into the fragment shader.
	texCoord = vec2(gl_MultiTexCoord0);
}


