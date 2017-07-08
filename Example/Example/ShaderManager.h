#pragma once
#include "GLShader.h"
#include "Config.h"
#include <vector>
class ShaderManager
{
public:
	static int CreateShaderSet(char* vsrc, char* fsrcs, unsigned long sig);
	static std::vector<Shader*> GetShaderSetBySignature(unsigned long sig, char* VSPath, char* FSPath);
	static std::vector<std::vector<Shader*>> m_shaders;
	static std::vector<Shader*> m_FXShaders;
	ShaderManager();
	~ShaderManager();
};

