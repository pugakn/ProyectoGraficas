#include "BMPParser.h"
#include <boost/iostreams/device/mapped_file.hpp>
bool BMPParser::LoadFile(const char * fileName)
{
	boost::iostreams::mapped_file_source file;
	file.open(fileName);
	if (file.is_open()) {
		info = const_cast<char*>(file.data());
		if (!info[0] == 'B' && info[1] == 'M') 
		{
			//Archivo no soportado
			return false;
		}
		imgWidth = *reinterpret_cast<uint32_t*>(&info[18]);
		imgHeight = *reinterpret_cast<uint32_t*>(&info[22]);
		bitsPerPixel = *reinterpret_cast<uint16_t*>(&info[28]);
		offset = *reinterpret_cast<uint32_t*>(&info[10]);
		//int size = (imgWidth * imgHeight * bitsPerPixel );
		bmpSize = *reinterpret_cast<uint32_t*>(&info[34]);//Tamaño del buffer

		m_bmpData = new unsigned char[bmpSize];
		for (int i = 0; i < bmpSize; i++) {
			m_bmpData[i] = info[offset + i];
		}
		if (bitsPerPixel == 24 || bitsPerPixel == 32)
		{
				for (int i = 0; i < bmpSize; i += 3) {
					unsigned char temp = m_bmpData[i];
					m_bmpData[i] = m_bmpData[i + 2];
					m_bmpData[i + 2] = temp;
				}
		}
		//m_bmpData = const_cast<char*>(&info[offset]);//Buffer
		file.close();
		return true;
	}

}


void BMPParser::Deallocate()
{
	m_bmpData;
}
