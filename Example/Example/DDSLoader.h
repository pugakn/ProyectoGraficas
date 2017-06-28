#pragma once
#include <vector>
#ifndef _WINDEF_
typedef unsigned long DWORD;
typedef unsigned char BYTE;
#endif
//Pixel Format Flags
#define DDPF_ALPHAPIXELS            0x00000001 
#define DDPF_ALPHA					0x00000002 
#define DDPF_FOURCC                 0x00000004  //Texture contains compressed RGB data; dwFourCC contains valid data.
#define DDPF_RGB                    0x00000040  //Texture contains uncompressed RGB data; dwRGBBitCount and the RGB masks (dwRBitMask, dwGBitMask, dwBBitMask) contain valid data.

// Header Flags
#define DDSD_CAPS                   0x00000001 
#define DDSD_HEIGHT                 0x00000002 
#define DDSD_WIDTH                  0x00000004 
#define DDSD_PITCH                  0x00000008 
#define DDSD_PIXELFORMAT            0x00001000 
#define DDSD_MIPMAPCOUNT            0x00020000 
#define DDSD_LINEARSIZE             0x00080000 
#define DDSD_DEPTH					0x00800000

//  Header Caps1
#define DDSCAPS_COMPLEX             0x00000008 
#define DDSCAPS_TEXTURE             0x00001000 
#define DDSCAPS_MIPMAP              0x00400000 

//  Header Caps2
#define DDSCAPS2_CUBEMAP            0x00000200 
#define DDSCAPS2_CUBEMAP_POSITIVEX  0x00000400 
#define DDSCAPS2_CUBEMAP_NEGATIVEX  0x00000800 
#define DDSCAPS2_CUBEMAP_POSITIVEY  0x00001000 
#define DDSCAPS2_CUBEMAP_NEGATIVEY  0x00002000 
#define DDSCAPS2_CUBEMAP_POSITIVEZ  0x00004000 
#define DDSCAPS2_CUBEMAP_NEGATIVEZ  0x00008000 
#define DDSCAPS2_VOLUME             0x00200000 


struct DDS_PIXELFORMAT {
	DWORD dwSize; //32 bytes
	DWORD dwFlags;
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwABitMask;
};
typedef struct {
	DWORD			DDSMagic;
	DWORD           dwSize;
	DWORD           dwFlags;
	DWORD           dwHeight;
	DWORD           dwWidth;
	DWORD           dwPitchOrLinearSize;
	DWORD           dwDepth;
	DWORD           dwMipMapCount;
	DWORD           dwReserved1[11];//Unused
	DDS_PIXELFORMAT ddspf;
	DWORD           dwCaps;
	DWORD           dwCaps2;
	DWORD           dwCaps3; //Unused
	DWORD           dwCaps4; //Unused
	DWORD           dwReserved2;//Unused
} DDS_HEADER;

struct DDSData
{
	DDS_HEADER *header; //128bytes + 4
	BYTE *bdata;
	//BYTE *bdata2;
};
struct CubeMapData
{
	DWORD width;
	DWORD height;
	DWORD mipMapLevels;
	std::vector<BYTE> pixelData;
};
class DDSLoader
{
public:
	enum DDS_FORMAT
	{
		BGRA8F_CUBEMAP,
		CBGRA8F_CUBEMAP
	};
	enum DDS_COMPRESION_TYPE
	{
		DXT1,
		DXT2,
		DXT3,
		DXT4,
		DXT5
	};
private:
	DDS_FORMAT GetFormat();
	DDS_COMPRESION_TYPE GetCompresionType();
	static const DWORD  DDSMagic = 0x20534444;
	DDSData data;
public:
	CubeMapData cubeMapData;
	bool Load(char *filePath);
	DDSLoader();
	~DDSLoader();
};

