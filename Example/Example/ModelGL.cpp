#include "ModelGL.h"
#ifdef USING_GL_COMMON
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include "Timer.h"
#include <iostream>
#include "TextureGL.h"
#include "GLShader.h"

#include "ShaderManager.h"


void ModelGL::SetFileName(char * fileName)
{
	m_fileName = std::string(fileName);
}
void ModelGL::Create()
{
	//------------------------------------------------------------------------//
	//Leer Archivo .X
	Timer timer;
	timer.Init();
	if (!parser.LoadFile(m_fileName.c_str()))
	{
		std::cout << "Error: No se pudo cargar el modelo" << std::endl;
		return;
	}
	timer.Update();
	std::cout << "Archivo cargado en: " << timer.GetDTSecs() << " segundos..." << std::endl;
	//-------------------------------------------------------------------------//
	//Iterar cada Mesh y subsets
	for (auto &meshIt: parser.m_meshes)
	{	
		m_meshInfo.push_back(MeshInfo());
		for (auto &subsetIt : meshIt.m_subsets)
		{
			m_meshInfo.back().subsetInfo.push_back(SubsetInfo());
			//Crear shaders
				if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_NORMAL)
					m_meshInfo.back().subsetInfo.back().sig |=  Shader::HAS_NORMALS;
				if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD0)
					m_meshInfo.back().subsetInfo.back().sig |= Shader::HAS_TEXCOORD0;
				if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD1)
					m_meshInfo.back().subsetInfo.back().sig |= Shader::HAS_TEXCOORD1;
				if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TANGENT)
					m_meshInfo.back().subsetInfo.back().sig |= Shader::HAS_TANGENTS;
				if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_BINORMAL)
					m_meshInfo.back().subsetInfo.back().sig |= Shader::HAS_BINORMALS;
				//if (subsetIt.m_effects.m_glossMap != "")
				//	Defines += "#define USE_GLOSS_MAP\n\n";
				if (subsetIt.m_effects.m_normalMap != "")
					m_meshInfo.back().subsetInfo.back().sig |= Shader::USE_NORMAL_MAP;
				if (subsetIt.m_effects.m_specularMap != "")
					m_meshInfo.back().subsetInfo.back().sig |= Shader::USE_SPEC_MAP;
				if (!useLight)
					m_meshInfo.back().subsetInfo.back().sig |= Shader::NOT_LIGHT;


				auto  set = ShaderManager::GetShaderSetBySignature(m_meshInfo.back().subsetInfo.back().sig);
				m_meshInfo.back().subsetInfo.back().m_shaderSet = set;
				m_shaderType = Shader::TYPE::G_FORWARD_PASS;
				GLShader * glDefaultShader = (GLShader*)(m_meshInfo.back().subsetInfo.back().m_shaderSet)[m_shaderType];
				m_meshInfo.back().subsetInfo.back().shadersID = glDefaultShader->ShaderID;

			if (glDefaultShader->vshader_id == 0 || glDefaultShader->fshader_id == 0)
			{
				m_meshInfo.back().subsetInfo.back().shadersID = Tools::DefaultShaderID;
				//glLinkProgram(m_meshInfo.back().subsetInfo.back().shadersID);
				glUseProgram(m_meshInfo.back().subsetInfo.back().shadersID);
			}
			else
			{
				//glLinkProgram(m_meshInfo.back().subsetInfo.back().shadersID);
				glUseProgram(m_meshInfo.back().subsetInfo.back().shadersID);
				//=========================== Create Textures ===============================
				m_meshInfo.back().subsetInfo.back().textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "diffuse"));
				int textureID = Tools::LoadTexture(subsetIt.m_effects.m_difusePath.c_str());
				m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(textureID);
				if (subsetIt.m_effects.m_glossMap != "")
				{
					m_meshInfo.back().subsetInfo.back().textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "glossMap"));
					textureID = Tools::LoadTexture(subsetIt.m_effects.m_glossMap.c_str());
					m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(textureID);
				}
				if (subsetIt.m_effects.m_normalMap != "")
				{
					m_meshInfo.back().subsetInfo.back().textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "normalMap"));
					textureID = Tools::LoadTexture(subsetIt.m_effects.m_normalMap.c_str());
					m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(textureID);
				}
				if (subsetIt.m_effects.m_specularMap != "")
				{
					m_meshInfo.back().subsetInfo.back().textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "specularMap"));
					textureID = Tools::LoadTexture(subsetIt.m_effects.m_specularMap.c_str());
					m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(textureID);
				}
			}
			//Generar buffer de Indices
			glGenBuffers(1, &m_meshInfo.back().subsetInfo.back().IB);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshInfo.back().subsetInfo.back().IB);
#if USING_32BIT_IB
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, subsetIt.m_indexBuffer.size() * sizeof(unsigned int), &(subsetIt.m_indexBuffer[0]), GL_STATIC_DRAW);
#else
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, subsetIt.m_indexBuffer.size() * sizeof(unsigned short), &(subsetIt.m_indexBuffer[0]), GL_STATIC_DRAW);
#endif // USING_32BIT_IB
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	//---------------------------------------------------------------//
	//Generar buffer de vertices
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, parser.m_vertexSize * sizeof(vertexStruct), &parser.m_vbo[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//===================================Generar Wireframe==================================
	//Shaders
	//Attach Shaders
	wire.ShaderID = Tools::DefaultShaderID;

	glLinkProgram(wire.ShaderID);
	//glUseProgram(wire.ShaderID);

	//Obtener locaciones de Attributes
	wire.VertexAttribLoc = glGetAttribLocation(wire.ShaderID, "Vertex");

	//Obtener locaciones de Uniforms
	wire.MatWorldViewProjUniformLoc = glGetUniformLocation(wire.ShaderID, "WVP");
	
	std::vector<unsigned short> fullIndex;
	for (auto &it : parser.m_vbo, parser.m_meshes)
	{
		fullIndex.insert(fullIndex.end(), it.m_indexBuffer.begin(), it.m_indexBuffer.end());
	}
	wireframe.CreateWireframe( fullIndex);
	glGenBuffers(1, &wire.IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wire.IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, wireframe.m_indexBuffer.size() * sizeof(unsigned short), &(wireframe.m_indexBuffer[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//Liberar Memoria
	parser.Deallocate();
	transform = Identity();


	//delete[] vsSourceP;
	//delete[] fsSourceP;
}

void ModelGL::Transform(float * t)
{
	transform = t;
}

void ModelGL::Draw(float *t)
{

	if (t)
		transform = t;

	Matrix4D VP = Matrix4D(pScProp->pCameras[0]->VP);
	Matrix4D WVP = transform*VP;
	//------------------------------------------------------------------//
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	switch (pScProp->renderMode)
	{
	case RM::RenderMode::SOLID:
	{
		DrawMeshes(VP,WVP);
		break;
	}
	case RM::RenderMode::WIREFRAME:
		DrawWireframe(VP, WVP);
		break;
	case RM::RenderMode::SOLID_WIREFRAME:
		DrawMeshes(VP,WVP);
		DrawWireframe(VP,WVP);
		break;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//----------------------------------------------------------------//
}

inline void ModelGL::DrawMeshes(const Matrix4D & VP, const Matrix4D & WVP)
{
	for (size_t i = 0; i < parser.m_meshes.size(); i++)
	{
		for (size_t j = 0; j < parser.m_meshes[i].m_subsets.size(); j++)
		{
			SubsetInfo* sIt = &m_meshInfo[i].subsetInfo[j];
			//Set actual shader
			glUseProgram(sIt->shadersID);
			//Set Uniforms
			Matrix4D WV = transform * pScProp->pCameras[0]->m_view;
			auto locs = ((GLShader*)(sIt->m_shaderSet[m_shaderType]))->m_locs;
			glUniformMatrix4fv(locs.matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);
			if (locs.matWorldViewUniformLoc != -1)
				glUniformMatrix4fv(locs.matWorldViewUniformLoc, 1, GL_FALSE, &WV.m[0][0]);
			if (locs.matWorldUniformLoc != -1)
				glUniformMatrix4fv(locs.matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
			if (locs.lightLoc != -1)
				glUniform3fv(locs.lightLoc, 1, &pScProp->Lights[0].Position.x);
			if (locs.lightColLoc != -1)
				glUniform3fv(locs.lightColLoc, 1, &pScProp->Lights[0].Color.r);
			if (locs.camPosLoc != -1)
				glUniform3fv(locs.camPosLoc, 1, &pScProp->pCameras[0]->m_pos.x);
			if (locs.specExpLoc != -1)
				glUniform1fv(locs.specExpLoc, 1, &pScProp->specExp);
			if (locs.attMaxLoc != 1)
				glUniform1fv(locs.attMaxLoc, 1, &pScProp->attMax);
			if (locs.camFarLoc != 1)
				glUniform1fv(locs.camFarLoc, 1, &pScProp->pCameras[0]->farPlane);
			//Enable Attributes
			glEnableVertexAttribArray(locs.vertexAttribLocs);
			if (locs.normalAttribLocs != -1)
				glEnableVertexAttribArray(locs.normalAttribLocs);
			if (locs.binormalAttribLocs != -1)
				glEnableVertexAttribArray(locs.binormalAttribLocs);
			if (locs.tangentAttribLocs != -1)
				glEnableVertexAttribArray(locs.tangentAttribLocs);
			if (locs.uvAttribLocs != -1)
				glEnableVertexAttribArray(locs.uvAttribLocs);
			//Specify Attributes location
			glVertexAttribPointer(locs.vertexAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(0));
			if (locs.normalAttribLocs != -1)
			{
				glVertexAttribPointer(locs.normalAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(16));
			}
			if (locs.tangentAttribLocs != -1)
			{
				glVertexAttribPointer(locs.tangentAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(32));
			}
			if (locs.binormalAttribLocs != -1)
			{
				glVertexAttribPointer(locs.binormalAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(48));
			}
			if (locs.uvAttribLocs != -1)
			{
				glVertexAttribPointer(locs.uvAttribLocs, 2, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(64));
			}
			//Bind Buffers
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sIt->IB);
			for (size_t k = 0; k < sIt->textInfo.IdsTex.size(); k++)
			{
				glActiveTexture(GL_TEXTURE0 + k);//Set Active texture unit
				glBindTexture(GL_TEXTURE_2D, sIt->textInfo.IdsTex[k]);
				glUniform1i(sIt->textInfo.IdTexUniformLocs[k], k); //Specify location
			}
#if USING_32BIT_IB
			glDrawElements(GL_TRIANGLES, parser.m_meshes[i].m_subsets[j].m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
#else
			glDrawElements(GL_TRIANGLES, parser.m_meshes[i].m_subsets[j].m_indexBuffer.size(), GL_UNSIGNED_SHORT, 0);
#endif // USING_32BIT_IB

			if (locs.normalAttribLocs != -1) {
				glDisableVertexAttribArray(locs.normalAttribLocs);
			}
			if (locs.uvAttribLocs != -1) {
				glDisableVertexAttribArray(locs.uvAttribLocs);
			}
			glDisableVertexAttribArray(locs.vertexAttribLocs);
			//Disable Shader
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glUseProgram(0);
		}
	}
}

inline void ModelGL::DrawWireframe(const Matrix4D & VP, const Matrix4D & WVP)
{
	for (size_t i = 0; i < parser.m_meshes.size(); i++)
	{
		for (size_t j = 0; j < parser.m_meshes[i].m_subsets.size(); j++)
		{
			//Set actual shader
			glUseProgram(wire.ShaderID);
			//Set Uniforms
			glUniformMatrix4fv(wire.MatWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);
			//Enable Attributes
			glEnableVertexAttribArray(wire.VertexAttribLoc);
			//Specify Attributes location
			glVertexAttribPointer(wire.VertexAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(0));
			//Bind Buffers
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wire.IB);
#if USING_32BIT_IB
			glDrawElements(GL_LINES, wireframe.m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
#else
			glDrawElements(GL_LINES, wireframe.m_indexBuffer.size(), GL_UNSIGNED_SHORT, 0);
#endif // USING_32BIT_IB
			glDisableVertexAttribArray(wire.VertexAttribLoc);
		}
	}
	//Disable Shader
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	
}

void ModelGL::Destroy()
{
	
}
void ModelGL::SetShaderType(Shader::TYPE type)
{
	if (m_shaderType != type)
	{
		m_shaderType = type;
		for (auto &meshIt : m_meshInfo)
		{
			for (auto &subsetIt : meshIt.subsetInfo)
			{
					subsetIt.shadersID =((GLShader*)(subsetIt.m_shaderSet)[type])->ShaderID;
					//glLinkProgram(subsetIt.shadersID);
					glUseProgram(subsetIt.shadersID);
					subsetIt.textInfo.IdTexUniformLocs.clear();
					subsetIt.textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(subsetIt.shadersID, "diffuse"));
					subsetIt.textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(subsetIt.shadersID, "glossMap"));
					subsetIt.textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(subsetIt.shadersID, "normalMap"));
					subsetIt.textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(subsetIt.shadersID, "specularMap"));
			}
		}
	}
}
ModelGL::~ModelGL()
{
}
#endif
