#pragma once
#include "GLShader.h"
#include "Config.h"
#include <vector>
class ShaderManager
{
public:
	//static unsigned long m_globalSignature;
	//static void SetGlobalSignature(unsigned long sig);
	static int CreateShaderSet(char* vsrc, char* fsrcs, unsigned long sig);
	static std::vector<Shader*> GetShaderSetBySignature(unsigned long sig);
	static std::vector<std::vector<Shader*>> m_shaders;
	ShaderManager();
	~ShaderManager();
};

