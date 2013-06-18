#version 120

// The fragment shader of the background displays a composite of the BG textures of
// the current and previous Tiles based on their states. The texture coordinate used
// is also animated to move the texture in a circle.

// The sampler that is bound to the BG texture of the previous Tile.
uniform sampler2D previous;

// The sampler that is bound to the BG texture of the current Tile.
uniform sampler2D current;

// The floating point state of the current Tile.
uniform float state;

// The time that the application has been running, in floating point seconds.
uniform int time;

// The smoothly interpolated texture coordinate received from the
// vertex shader.
varying vec2 texCoord;

// The period of the waves in the background.
float period = 1000.0;

// The amplitude of the waves in the background.
float amplitude = .025;

// The texel that we will be working with.
vec4 texel;

// Animates the texture by manipulating the texture coordinates.
vec2 animate(vec2 toAnimate);

// Returns a texel of the combination of the two BG textures, based on the Tiles' state.
vec4 getMixedTexel(vec2 location);

void main()
{
	texel = vec4(0);
	vec2 tex = texCoord;
	tex.y = 1-tex.y;
	tex = animate(tex);
	texel = getMixedTexel(tex);
	if(texel.a > 1) texel.a = 1;
	gl_FragColor = texel;
}

vec2 animate(vec2 toAnimate)
{
	// Should probably optimize this sometime.
	
	// Create an angle value.
	float angle =  (float(time)-(period/2)) / period;
	
	// Manipulate the incoming vec2.
	toAnimate.x += amplitude*sin(angle+(gl_FragCoord.y/period));
	return toAnimate;
}

vec4 getMixedTexel(vec2 location)
{
	if(state > .9)
	{
		return texture2D(current, location);
	}
	else if(state < .1)
	{
		return texture2D(previous, location);
	}
	else return (texture2D(previous, location)*(1-state)) + (texture2D(current, location)*(state));
}
	
	