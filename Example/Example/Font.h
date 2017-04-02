#pragma once
#include <vector>
class Font
{
	struct CharInfo
	{
		uint32_t id;
		uint16_t x;
		uint16_t y;
		uint16_t width;
		uint16_t height;
		int16_t xoffset;
		int16_t yoffset;
		int16_t xadvance;
		uint8_t page;
		uint8_t chnl;
	};
private:
	char* info;
public:
	bool LoadFile(const char * fileName);
	CharInfo* m_charInfo;
	void Delete();

	Font();
	~Font();
};

