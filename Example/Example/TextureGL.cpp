/*********************************************************
* Copyright (C) 2017 Daniel Enriquez (camus_mm@hotmail.com)
* All Rights Reserved
*
* You may use, distribute and modify this code under the
* following terms:
* ** Do not claim that you wrote this software
* ** A mention would be appreciated but not needed
* ** I do not and will not provide support, this software is "as is"
* ** Enjoy, learn and share.
*********************************************************/

#include "TextureGL.h"

#ifdef USING_OPENGL_ES20
#include <GLES2\gl2.h>
#include <GLES2\gl2ext.h>
#elif defined(USING_OPENGL_ES30)
#include <GLES3\gl31.h>
#elif defined(USING_OPENGL)
#include <GL\glew.h>
#endif

#ifdef USING_GL_COMMON
void	TextureGL::SetTextureParams(unsigned int &target) {

	glBindTexture(GL_TEXTURE_2D, target);

	unsigned int glFiltering = 0;
	unsigned int glWrap = 0;
	

	glFiltering = GL_NEAREST;
	glWrap = GL_CLAMP_TO_EDGE;
	
	if(params & TEXT_BASIC_PARAMS::MIPMAPS)
		glFiltering = GL_LINEAR_MIPMAP_LINEAR;

	if (params & TEXT_BASIC_PARAMS::CLAMP_TO_EDGE)
		glWrap = GL_CLAMP_TO_EDGE;	
	
	if (params & TEXT_BASIC_PARAMS::TILED)
		glWrap = GL_REPEAT;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFiltering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFiltering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap);

#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL)
	int Max = 1;
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Max);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Max);
#endif

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureGL::GetFormatBpp(unsigned int &props, unsigned int &glFormat, unsigned int &bpp) {

}

void TextureGL::LoadAPITexture(const unsigned char* buffer) {
	unsigned int id;
	unsigned int glFormat = 0;
	unsigned int glChannel = GL_UNSIGNED_BYTE;
	unsigned int glTarget = GL_TEXTURE_2D;

	if (this->props&TEXT_BASIC_FORMAT::CH_ALPHA)
		glFormat = GL_ALPHA;
	else if (this->props&TEXT_BASIC_FORMAT::CH_RGB)
		glFormat = GL_RGB;
	else if (this->props&TEXT_BASIC_FORMAT::CH_RGBA)
		glFormat = GL_RGBA;

	glGenTextures(1, &id);
	glBindTexture(glTarget, id);

	if (this->x % 4 != 0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	else
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	
	glTexImage2D(glTarget, 0, glFormat, this->x, this->y, 0, glFormat, glChannel, (void*)(buffer));

	glGenerateMipmap(glTarget);
	SetTextureParams(glTarget);
	this->id = static_cast<unsigned short>(id);

}

void TextureGL::LoadAPITextureCompressed(unsigned char* buffer) {
	
}
void TextureGL::LoadAPICubeMap(unsigned char * buffer)
{
	unsigned int id;
	unsigned int glFormat = 0;
	unsigned int glChannel = GL_UNSIGNED_BYTE;
	unsigned int glTarget = GL_TEXTURE_CUBE_MAP;

	glFormat = GL_RGBA;
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	
	glGenTextures(1, &id);
	glBindTexture(glTarget, id);
	if (this->x % 4 != 0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	else
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	for (int i = 0; i<6; i++) {
		int nx = this->x;
		int ny = this->y;
		size = x* y * 4;
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT + i, 0, glFormat, nx, ny, 0, glFormat, glChannel, (void*)(buffer));
		buffer += size;
		for (int y = 1; y < mipmaps; y++) {
			//gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT + i, glFormat, nx, ny,  glFormat, glChannel, (void*)(buffer));
			nx = (nx + 1) >> 1;
			ny = (ny + 1) >> 1;
			size = nx * ny * 4;
			if (size < 8) size = 8;
			buffer += size;
		}
	}
	if (mipmaps == 1)
		glGenerateMipmap(glTarget);



	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	this->id = static_cast<unsigned short>(id);
	glDisable(GL_TEXTURE_CUBE_MAP_EXT);
}
#endif