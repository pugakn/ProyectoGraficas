#ifndef UAD_TEXTURE_H
#define UAD_TEXTURE_H

enum TEXT_BASIC_FORMAT{
	CH_ALPHA = 1,
	CH_RGB	 = 2,
	CH_RGBA  = 4
};

class Texture {
public:
	Texture() :
		size(0),
		props(0),
		params(0),
		x(0),
		y(0),
		id(0),
		bounded(0),
		mipmaps(0)
	{

	}


	~Texture() {

	}
	
	int				LoadTexture(char *fn);

	virtual void	LoadAPITexture(unsigned char* buffer) = 0;
	virtual void	LoadAPITextureCompressed(unsigned char* buffer) = 0;

	virtual void	SetTextureParams(unsigned int &params, unsigned int &target) = 0;
	virtual void	GetFormatBpp(unsigned int &props, unsigned int &format, unsigned int &bpp) = 0;

	char			optname[128];
	unsigned int	size;
	unsigned int	props;
	unsigned int	params;
	unsigned short	x, y;
	unsigned short	id;
	unsigned char	bounded;
	unsigned char	mipmaps;
};

#endif