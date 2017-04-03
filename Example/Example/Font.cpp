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
		nextBlock +=  1;
		uint32_t blockSize = *reinterpret_cast<uint32_t*>(&info[nextBlock]);
		nextBlock += 4;
		//Cargar Info
		//nextBlock += blockSize + 1;
		//blockSize = *reinterpret_cast<uint32_t*>(&info[nextBlock]) ;
		//nextBlock += 4;
		//Cargar Common
		nextBlock += blockSize + 1;
		blockSize = *reinterpret_cast<uint32_t*>(&info[nextBlock]) ;
		nextBlock += 4;
		//Cargar nombre
		nextBlock += blockSize + 1;
		blockSize = *reinterpret_cast<uint32_t*>(&info[nextBlock]) ;
		nextBlock += 4;
		//Cargar informacion de caracteres
		nextBlock += blockSize + 1;
		blockSize = *reinterpret_cast<uint32_t*>(&info[nextBlock]);
		nextBlock += 4;
		int CharInfoSize = blockSize / sizeof(CharInfo);
		//m_charInfo = new CharInfo[CharInfoSize];
		//m_charInfo.resize(CharInfoSize);

		m_charInfo.assign (reinterpret_cast<CharInfo*>(&info[nextBlock]), reinterpret_cast<CharInfo*>(&info[nextBlock])+CharInfoSize);
		//Cargar Kerning


		file.close();
		return true;
	}

}

void Font::Delete()
{
	//delete[] m_charInfo;
}


Font::Font()
{
}


Font::~Font()
{
}
