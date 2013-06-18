#version 120

// The Tile's fragment shader does the texturing based on the state of the texture.
// The texture displayed in the Tile is determined by the floating point state variable.
// When the state is at 0, the inactive texture is displayed, and at 1, the active
// texture is displayed.
// The Tile's frame is drawn here if necessary as well. The frame shrinks the screenshot
// so that it can fit on the quad.

// The texture of the frame. This sampler may or may not be used.
uniform sampler2D frameTexture;

// A boolean integer that determines whether or not we draw a frame.
uniform int hasFrame;

// The active texture sampler.
uniform sampler2D activeTexture;

// The inactive texture sampler.
uniform sampler2D inactiveTexture;

// The state of the Tile.
uniform float state;

// The current application time.
uniform int time;

// The time the Tile went to active.
uniform int activeTime;

// The delay time before the Tile fades when active.
int delay = 2000;

// The fade-out time.
int fadeout = 750;

// The texture coordinate from the vertex shader.
centroid varying vec2 texCoord;

// The texel that we will be working with.
vec4 texel;

vec4 getScreenshotTexel(vec2 location);
vec4 getFrameTexel(vec2 location);
vec2 scaleTexelCoordToWithinFrame(vec2 toScale);
vec4 getOpacityBasedOnTimeActive(vec4 texel);

//
void main(void)
{
	// Initialize our output texel.
	texel = vec4(0);
	
	// Create a copy of texCoord.
	vec2 coord = texCoord;
	
	// If this Tile has a frame, we fill our output texel
	// with both texels from the frame and from the 
	// screenshot composite.
	if(hasFrame == 1)
	{
		texel = getFrameTexel(coord);
		coord = scaleTexelCoordToWithinFrame(coord);
	}
	texel += getScreenshotTexel(coord);
	if(texel.a > 1.0) texel.a = 1;
	gl_FragColor = getOpacityBasedOnTimeActive(texel);
}

// Returns a texel of the screenshot, composited based on the current state.
vec4 getScreenshotTexel(vec2 location)
{
	if(location.x > 1 || location.x < 0 || location.y > 1 || location.y < 0)
	{
		return vec4(0);
	}
	else if(state > .9)
	{
		return texture2D(activeTexture, location);
	}
	else if(state < .1)
	{
		return texture2D(inactiveTexture, location);
	}
	else return (texture2D(activeTexture, location) * state)+(texture2D(inactiveTexture, location) *(1-state));
}

// Returns a texel from the frame texture.
vec4 getFrameTexel(vec2 location)
{
	return texture2D(frameTexture, location);
}

// This scaling is hard-coded to work with the Sonic 3 frame.
vec2 scaleTexelCoordToWithinFrame(vec2 toScale)
{
	// HORIZONTAL
	// multiply size * 1.2
	// move left by 19.6 %
	toScale.x *= 1.1975;
	toScale.x -= .0625;
	
	// VERTICAL
	// multiply size * 1.21333
	// move down by 9.4375 %
	toScale.y *= 1.272725;
	toScale.y -= .187375;
	
	return toScale;
}

vec4 getOpacityBasedOnTimeActive(vec4 texel)
{
	if(time-activeTime > delay)
	{
		if(state > .1)
		{
			texel.a -= (float(time-activeTime-delay)/float(fadeout)) * state;
		}
		/*else if(state < .1)
		{
			texel.a -= float(time-activeTime-delay)/float(fadeout*2);
			if(texel.a < .5 && ( texel.r > 0 || texel.g > 0 || texel.b > 0)) texel.a = .5;
		}*/
	}
	return texel;
}
	

