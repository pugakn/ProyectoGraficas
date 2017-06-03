#pragma once
#include "GLShader.h"
#include <vector>
class ShaderManager
{
public:
	static unsigned long m_globalSignature;
	static void SetGlobalSignature(unsigned long sig);
	static int CreateShader(char* vsrc, char* fsrcs, unsigned long sig);
	static Shader* GetShaderBySignature(unsigned long sig);
	static std::vector<Shader*> m_shaders;
	ShaderManager();
	~ShaderManager();
};

