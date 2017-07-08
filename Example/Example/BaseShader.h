#pragma once
#include <string>
#include "Config.h"
#include "UtilsGL.h"
#include <string>
class Shader
{
public:
	enum TYPE
	{
		G_BUFF_PASS,
		G_FORWARD_PASS,
		G_SHADOW_PASS,
		G_DEFERRED_PASS,
		COUNT
	};
	enum SIGNATURE
	{
		//TYPES
		//GBUFF_PASS = 1,
		//GFORWARD_PASS = GBUFF_PASS << 1 ,
		//GSHADOW_PASS = GBUFF_PASS << 11,

		HAS_NORMALS = 1,
		HAS_TANGENTS = HAS_NORMALS << 1,
		HAS_BINORMALS = HAS_NORMALS << 2,
		HAS_TEXCOORD0 = HAS_NORMALS << 3,
		HAS_TEXCOORD1 = HAS_NORMALS << 4,

		USE_NORMAL_MAP = HAS_NORMALS << 5,
		USE_SPEC_MAP = HAS_NORMALS << 6,
		USE_GLOSS_MAP = HAS_NORMALS << 7,
		NOT_LIGHT = HAS_NORMALS << 8

	};
	void Load(char* vsStr, char* fsStr, unsigned long signature, Shader::TYPE type);
	unsigned long m_signature;
	Shader::TYPE m_type;
	virtual void LoadAPI() = 0;
	virtual ~Shader() {};
	bool errorShader;
	std::string VSPath;
protected:
	std::string vstr;
	std::string fstr;
	std::string Defines;
};
