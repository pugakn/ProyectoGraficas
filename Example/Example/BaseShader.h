#pragma once
#include <string>
#include "Config.h"
#include "UtilsGL.h"
class Shader
{
public:
	enum SIGNATURE
	{
		//TYPES
		GBUFF_PASS = 1,
		GFORWARD_PASS = GBUFF_PASS << 1 ,

		HAS_NORMALS = GBUFF_PASS << 2,
		HAS_TANGENTS = GBUFF_PASS << 3,
		HAS_BINORMALS = GBUFF_PASS << 4,
		HAS_TEXCOORD0 = GBUFF_PASS << 5,
		HAS_TEXCOORD1 = GBUFF_PASS << 6,

		USE_NORMAL_MAP = GBUFF_PASS << 7,
		USE_SPEC_MAP = GBUFF_PASS << 8,
		USE_GLOSS_MAP = GBUFF_PASS << 9,
		NOT_LIGHT = GBUFF_PASS << 10

	};
	void Load(char* vsStr, char* fsStr, unsigned long signature);
	unsigned long m_signature;
	virtual void LoadAPI() = 0;
	virtual ~Shader() {};
protected:
	std::string vstr;
	std::string fstr;
	std::string Defines;
	bool errorShader;
};
