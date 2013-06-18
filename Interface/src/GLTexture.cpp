#include "GLTexture.h"


GLTexture::GLTexture(void)
{
}


GLTexture::~GLTexture(void)
{
	deleteTexture();
}

void GLTexture::createEmptyTexture(int width, int height, GLenum format)
{
	w = width;
	h = height;

	// Generate our texture ID.
	glGenTextures(1, &texture);

	// Generate a sampler ID.
	glGenSamplers(1, &sampler);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	// The internal format of a texture should not be out of order (BGRA), so 
	// we intercept those here.
	if(format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	else if(format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
}

bool GLTexture::loadTextureImage(char * filename, bool genMipMaps, GLenum format)
{
	// Create a FREE_IMAGE_FORMAT to store the image format.
	FREE_IMAGE_FORMAT fmt = FIF_UNKNOWN;

	// A pointer to the image for when it is loaded.
	FIBITMAP * imagePointer = NULL;

	// A pointer to the image bitmap for when it is loaded.
	BYTE * bitmap = NULL;

	// Image width and height.
	unsigned int width = 0, height = 0;

	// First we try to deduce the file format from the file signature.
	fmt = FreeImage_GetFileType(filename, 0);
	// If that didn't work, we try to get the format from the file extension.
	if(fmt == FIF_UNKNOWN)
		fmt = FreeImage_GetFIFFromFilename(filename);
	// If that didn't work, oh well.
	if(fmt == FIF_UNKNOWN)
		return false;
	
	// Make sure that the FreeImage plugin has reading capabilities, and if it does,
	// load the file.
	if(FreeImage_FIFSupportsReading(fmt))
		imagePointer = FreeImage_Load(fmt, filename);
	// If the file wasn't loaded, return.
	if(imagePointer == NULL)
		return false;

	// Retrieve the image data.
	bitmap = FreeImage_GetBits(imagePointer);

	// Get the width and height of the image.
	width = FreeImage_GetWidth(imagePointer);
	height = FreeImage_GetHeight(imagePointer);

	// Make sure none of these operations failed.
	if(bitmap == NULL || width == 0 || height == 0)
		return false;

	// Generate our texture ID.
	glGenTextures(1, &texture);

	// Generate a sampler ID.
	glGenSamplers(1, &sampler);

	glBindTexture(GL_TEXTURE_2D, texture);

	if(genMipMaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}

	// The internal format of a texture should not be out of order (BGRA), so 
	// we intercept those here.
	if(format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, bitmap);
	else if(format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, bitmap);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bitmap);

	// Unload FreeImage's copy of the data.
	FreeImage_Unload(imagePointer);
	return true;

}

void GLTexture::bindTexture(int textureUnit)
{
	// Make the texture unit <TextureUnit> the active texture, meaning that all
	// subsequent texture calls, such as glBindTexture() will affect this
	// texture unit.
	glActiveTexture(GL_TEXTURE0+textureUnit);

	// Bind our texture to the GL_TEXTURE_2D target of our active texture unit.
	glBindTexture(GL_TEXTURE_2D, texture);

	// We also bind our sampler to this texture unit so that we can access
	// this texture within shaders.
	glBindSampler(textureUnit, sampler);

}

void GLTexture::setFiltering(GLenum mag, GLenum min)
{
	// Bind to our sampler so that we can edit the interpolation attributes.
	// We bind it to the 0th textre target since we don't need it to be attached
	// to a texture unit to edit its properties.
	glBindSampler(0, sampler);

	// Set the magnification and minifcation filters.
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, mag);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, min);
}

void GLTexture::setSamplerParameter(GLenum parameter, GLenum value)
{
	// This one is a straight pass-through.
	glSamplerParameteri(sampler, parameter, value);
}

void GLTexture::deleteTexture(void)
{
	// Deletes the texture object from GPU memory if it ever existed.
	if(sampler != 0) glDeleteSamplers(1, &sampler);

	// Deletes the sampler object from GPU memory if it ever existed.
	if(texture != 0) glDeleteTextures(1, &texture);
}

GLuint GLTexture::getTextureID(void)
{
	return texture;
}

GLuint GLTexture::getSamplerID(void)
{
	return sampler;
}

int GLTexture::getWidth(void)
{
	return w;
}

int GLTexture::getHeight(void)
{
	return h;
}



