#pragma once
class BMPParser
{
private:
	//boost::iostreams::mapped_file_source file;
	char* info;
	size_t fileSize;
	size_t imgWidth;
	size_t imgHeight;
	size_t bitsPerPixel;
	size_t offset;
public:
	unsigned char* m_bmpData;
	size_t bmpSize;
	bool LoadFile(const char* fileName);
	void Deallocate();
	BMPParser() {};
	~BMPParser() {};
};

