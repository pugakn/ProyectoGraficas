#include "BaseShader.h"


void Shader::Load(char* vsStr, char* fsStr, unsigned long signature, Shader::TYPE type)
{
	Defines.clear();
	m_signature = signature;
	m_type = type;
	errorShader = false;
	char *vsSource = file2string(vsStr);
	char *fsSource = file2string(fsStr);
	if (!vsSource || !fsSource)
		errorShader = true;
	else {
		vstr = std::string(vsSource);
		fstr = std::string(fsSource);
	}
	if (vsSource != NULL && fsSource != NULL)
	{
		//Types
		if (m_type == TYPE::G_BUFF_PASS)
			Defines += "#define G_BUFF_PASS \n\n";
		else if (m_type == TYPE::G_FORWARD_PASS)
			Defines += "#define G_FORWARD_PASS \n\n";
		else if (m_type == TYPE::G_SHADOW_PASS)
			Defines += "#define G_SHADOW_PASS \n\n";
		//Signature 
		if (signature & SIGNATURE::HAS_NORMALS)
			Defines += "#define USE_NORMALS\n\n";
		if (signature & SIGNATURE::HAS_TEXCOORD0)
			Defines += "#define USE_TEXCOORD0\n\n";
		if (signature & SIGNATURE::HAS_TEXCOORD1)
			Defines += "#define USE_TEXCOORD1\n\n";
		if (signature & SIGNATURE::HAS_TANGENTS)
			Defines += "#define USE_TANGENTS\n\n";
		if (signature &  SIGNATURE::HAS_BINORMALS)
			Defines += "#define USE_BINORMALS\n\n";



		if (signature & SIGNATURE::USE_GLOSS_MAP)
			Defines += "#define USE_GLOSS_MAP\n\n";
		if (signature & SIGNATURE::USE_NORMAL_MAP)
			Defines += "#define USE_NORMAL_MAP\n\n";
		if (signature & SIGNATURE::USE_SPEC_MAP)
			Defines += "#define USE_SPEC_MAP\n\n";

		if (!(signature & SIGNATURE::NOT_LIGHT)){
			Defines += "#define USE_PIXELLIGHTING \n\n";
			Defines += "#define USING_ATENUATION \n\n";
			Defines += "#define USE_SPECULAR_BLIN \n\n";
		}

		//Defines += "#define LINEAR_DEPTH \n\n";
#ifdef USING_OPENGL
		Defines += "#define lowp\n\n";
		Defines += "#define mediump\n\n";
		Defines += "#define highp\n\n";
#endif // USING_OPENGL
	}
	vstr = Defines + vstr;
	fstr = Defines + fstr;
	delete[] vsSource;
	delete[] fsSource;
	LoadAPI();
}
