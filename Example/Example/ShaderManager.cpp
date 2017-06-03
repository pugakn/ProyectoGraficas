#include "ShaderManager.h"


std::vector<Shader*> ShaderManager::m_shaders;
unsigned long ShaderManager::m_globalSignature = 0;
void ShaderManager::SetGlobalSignature(unsigned long sig)
{
	m_globalSignature = sig;
}
int ShaderManager::CreateShader(char * vsrc, char * fsrcs, unsigned long sig)
{
	Shader* sdhr = new GLShader();
	sdhr->Load(vsrc, fsrcs, sig | m_globalSignature);
	m_shaders.push_back(sdhr);
	return m_shaders.size() -1;
}

Shader * ShaderManager::GetShaderBySignature(unsigned long sig)
{
	for (auto &sh : m_shaders)
	{
		if (sh->m_signature == (sig | m_globalSignature))
			return sh;
	}
	return m_shaders[CreateShader("Shaders/VS_MeshPL.glsl","Shaders/FS_MeshPL.glsl",  sig | m_globalSignature)];
}

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}
