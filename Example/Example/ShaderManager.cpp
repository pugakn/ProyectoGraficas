#include "ShaderManager.h"
#include "D3DShader.h"


std::vector<std::vector<Shader*>> ShaderManager::m_shaders;
int ShaderManager::CreateShaderSet(char * vsrc, char * fsrcs, unsigned long sig)
{
	std::vector<Shader*> tmpSahders;
#ifdef USING_GL_COMMON
	tmpSahders.resize(Shader::TYPE::COUNT);
	for (size_t i = 0; i < Shader::TYPE::COUNT; i++)
	{
		tmpSahders[i] = new GLShader();
		tmpSahders[i]->VSPath = vsrc;
		tmpSahders[i]->Load(vsrc, fsrcs, sig,(Shader::TYPE)i);

	}
#else
	tmpSahders.resize(Shader::TYPE::COUNT);
	for (size_t i = 0; i < Shader::TYPE::COUNT; i++)
	{
		tmpSahders[i] = new D3DShader();
		tmpSahders[i]->Load(vsrc, fsrcs, sig, (Shader::TYPE)i);
	}
#endif
	m_shaders.push_back(tmpSahders);
	return m_shaders.size() -1;
}

std::vector<Shader*> ShaderManager::GetShaderSetBySignature(unsigned long sig, char* VSPath, char* FSPath)
{
	for (auto &sh : m_shaders)
	{
		if (sh.back()->m_signature == (sig) && sh.back()->VSPath == std::string(VSPath))
			return sh;
	}
	return m_shaders[CreateShaderSet(VSPath,FSPath,  sig )];
}

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}
