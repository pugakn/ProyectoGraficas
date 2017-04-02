#include "Font.h"
#include <boost/iostreams/device/mapped_file.hpp>
bool Font::LoadFile(const char * fileName)
{
	boost::iostreams::mapped_file_source file;
	file.open(fileName);
	if (file.is_open()) {
		info = const_cast<char*>(file.data());
		//Leer Bloques
		uint32_t nextBlock = 4;
		uint32_t blockSize = *reinterpret_cast<uint32_t*>(&info[nextBlock+1]);
		//Cargar Info
		nextBlock += blockSize;
		blockSize = *reinterpret_cast<uint32_t*>(&info[nextBlock + 1]);
		//Cargar Common
		nextBlock += blockSize;
		blockSize = *reinterpret_cast<uint32_t*>(&info[nextBlock + 1]);
		//Cargar nombre
		nextBlock += blockSize;
		blockSize = *reinterpret_cast<uint32_t*>(&info[nextBlock + 1]);
		//Cargar informacion de caracteres
		int CharInfoSize = blockSize / 20;
		CharInfo* buffer = new CharInfo[CharInfoSize];
		buffer = reinterpret_cast<CharInfo*>(&info[nextBlock + 2]);
		//Cargar Kerning


		file.close();
		return true;
	}

}

void Font::Delete()
{
	delete[] m_charInfo;
}


Font::Font()
{
}


Font::~Font()
{
}
