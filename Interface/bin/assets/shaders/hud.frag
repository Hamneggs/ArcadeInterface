#version 120

// The HUD really is only a quad with a texture on it, so the 
// fragment shader is quite simple.

// The sampler that is bound to the HUD texture.
uniform sampler2D hudTexture;

// The sampler that is bound to the INTRO texture.
uniform sampler2D introTexture;

// The smoothly interpolated texture coordinate received from the
// vertex shader.
centroid varying vec2 texCoord;

// The current time.
uniform int time;

// The number of milliseconds that the intro lasts.
int duration = 3000;

// The number of milliseconds that the transition lasts.
int transition = 1000;

vec4 getTexel(void)
{
	if(time < duration)
	{
		return texture2D(introTexture, vec2(texCoord.x, 1-texCoord.y));
	}
	else if(time < duration + transition)
	{
		vec4 white = vec4(1);
		vec4 introTexel = texture2D(introTexture, vec2(texCoord.x, 1-texCoord.y));
		vec4 hudTexel = texture2D(hudTexture, vec2(texCoord.x, 1-texCoord.y));
		
		float factor = 1-(float((transition)-(time-duration))/float(transition));
		
		if(factor < .33333)
		{
			factor *= 3;
			introTexel.r = (1*(factor)) + (introTexel.r * (1-factor));
			introTexel.g = (1*(factor)) + (introTexel.g * (1-factor));
			introTexel.b = (1*(factor)) + (introTexel.b * (1-factor));
			return introTexel;
		}
		else if(factor < .66666)
		{
			white.a *= 1-((factor-.333333)*3);
			return white;
		}
		else
		{
			hudTexel.a *= ((factor-.666666)*3);
			return hudTexel;
		}
		
	}
	else
	{
		return texture2D(hudTexture, vec2(texCoord.x, 1-texCoord.y));
	}
	
}
void main(void)
{
	gl_FragColor = vec4(0);
	gl_FragColor += getTexel();
}
