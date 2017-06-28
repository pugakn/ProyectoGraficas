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

std::vector<Shader*> ShaderManager::GetShaderSetBySignature(unsigned long sig)
{
	for (auto &sh : m_shaders)
	{
		if (sh.back()->m_signature == (sig))
			return sh;
	}
#ifdef USING_GL_COMMON
	return m_shaders[CreateShaderSet("Shaders/VS_MeshPL.glsl","Shaders/FS_MeshPL.glsl",  sig )];
#else
	return m_shaders[CreateShaderSet("Shaders/VS_MeshPL.hlsl", "Shaders/FS_MeshPL.hlsl", sig )];
#endif
}

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}
