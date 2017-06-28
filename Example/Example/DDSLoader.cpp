#include "DDSLoader.h"
#include <boost/iostreams/device/mapped_file.hpp>


DDSLoader::DDS_FORMAT DDSLoader::GetFormat()
{
	//TODO: More formats

	if (((data.header->ddspf.dwFlags & DDPF_RGB) &&
		(data.header->ddspf.dwFlags & DDPF_ALPHAPIXELS) &&
		(data.header->ddspf.dwRGBBitCount == 32) &&
		(data.header->ddspf.dwRBitMask == 0xff0000) &&
		(data.header->ddspf.dwGBitMask == 0xff00) &&
		(data.header->ddspf.dwBBitMask == 0xff) &&
		(data.header->ddspf.dwABitMask == 0xff000000U)))
	{
		return DDS_FORMAT::BGRA8F_CUBEMAP;
	}
	else if (data.header->ddspf.dwFlags & DDPF_FOURCC)//&& data.header->dwCaps2 & DDSCAPS2_CUBEMAP
	{
		return DDS_FORMAT::CBGRA8F_CUBEMAP;
	}
}

DDSLoader::DDS_COMPRESION_TYPE DDSLoader::GetCompresionType()
{
	if (data.header->ddspf.dwFourCC == '1TXD')
		return DDS_COMPRESION_TYPE::DXT1;
	else if (data.header->ddspf.dwFourCC == '2TXD')
		return DDS_COMPRESION_TYPE::DXT2;
	else if (data.header->ddspf.dwFourCC == '3TXD')
		return DDS_COMPRESION_TYPE::DXT3;
	else if (data.header->ddspf.dwFourCC == '4TXD')
		return DDS_COMPRESION_TYPE::DXT4;
	else if (data.header->ddspf.dwFourCC == '5TXD')
		return DDS_COMPRESION_TYPE::DXT5;
}

bool DDSLoader::Load(char * filePath)
{
	boost::iostreams::mapped_file_source file;
	file.open(filePath);
	if (file.is_open()) {
		char *ptr = const_cast<char*>(file.data());
		data.header = (DDS_HEADER*)&ptr[0];
		if (data.header->DDSMagic != DDSMagic || data.header->dwSize != 124)
		{
			//Archivo no soportado
			return false;
		}
		cubeMapData.width = data.header->dwWidth;
		cubeMapData.height = data.header->dwHeight;
		cubeMapData.mipMapLevels = data.header->dwMipMapCount;
		if ((cubeMapData.width & (cubeMapData.width - 1)))
		{
			file.close();
			return true;
		}
		if ((cubeMapData.height & (cubeMapData.height - 1))) {
			file.close();
			return true;
		}
		BYTE* pixelData = (BYTE*)&ptr[128];
		DDS_FORMAT format = GetFormat();
		switch (format)
		{
		case DDSLoader::BGRA8F_CUBEMAP:
			cubeMapData.pixelData.assign(pixelData,pixelData + (file.size()-128));
			for (size_t i = 0; i < cubeMapData.pixelData.size(); i+=4)
			{
				auto tmp = cubeMapData.pixelData[i];
				cubeMapData.pixelData[i] = cubeMapData.pixelData[i + 2];
				cubeMapData.pixelData[i + 2] = tmp;
			}
			break;
		case DDSLoader::CBGRA8F_CUBEMAP:
		{
			DDS_COMPRESION_TYPE compType = GetCompresionType();
		}
			break;
		default:
			break;
		}


		
		file.close();
		return true;
	}
}

DDSLoader::DDSLoader()
{
}


DDSLoader::~DDSLoader()
{
}
