#version 120

// The HUD really is only a quad with a texture on it, so the 
// fragment shader is quite simple.

// The sampler that is bound to the HUD texture.
uniform sampler2D hudTexture;

// The smoothly interpolated texture coordinate received from the
// vertex shader.
centroid varying vec2 texCoord;

void main(void)
{
	gl_FragColor = vec4(0);
	gl_FragColor += texture2D(hudTexture, vec2(texCoord.x, 1-texCoord.y));
}
