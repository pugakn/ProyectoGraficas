#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int		Texture::LoadTexture(char *fn){
	int x = 0, y = 0, channels = 0;
	unsigned char *buffer = stbi_load(fn, &x, &y, &channels, 0);

	if(!buffer)
		return -1;

	size = x*y*channels;
	bounded = 1;
	this->x = x;
	this->y = y;
	mipmaps = 1;
	this->params = params;

	switch (channels) {
		case 1: {
			props |= TEXT_BASIC_FORMAT::CH_ALPHA;
		}break;
		case 3: {
			props |= TEXT_BASIC_FORMAT::CH_RGB;
		}break;
		case 4: {
			props |= TEXT_BASIC_FORMAT::CH_RGBA;
		}break;
	}

	memcpy(&optname[0], fn, strlen(fn));
	optname[strlen(fn)] = '\0';

	LoadAPITexture(buffer);
	stbi_image_free(buffer);
	return id;
}
