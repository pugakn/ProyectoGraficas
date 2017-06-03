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

			m_meshInfo.back().subsetInfo.back().m_shader = (GLShader*)ShaderManager::GetShaderBySignature(m_meshInfo.back().subsetInfo.back().sig);
			m_meshInfo.back().subsetInfo.back().shadersID = ((GLShader*)m_meshInfo.back().subsetInfo.back().m_shader)->ShaderID;

			if (((GLShader*)m_meshInfo.back().subsetInfo.back().m_shader)->vshader_id == 0 || ((GLShader*)m_meshInfo.back().subsetInfo.back().m_shader)->fshader_id == 0)
			{
				m_meshInfo.back().subsetInfo.back().shadersID = Tools::DefaultShaderID;
				glLinkProgram(m_meshInfo.back().subsetInfo.back().shadersID);
				glUseProgram(m_meshInfo.back().subsetInfo.back().shadersID);

				m_meshInfo.back().subsetInfo.back().vertexAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Vertex");
				m_meshInfo.back().subsetInfo.back().matWorldViewProjUniformLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "WVP");
			}
			else
			{
				glLinkProgram(m_meshInfo.back().subsetInfo.back().shadersID);
				glUseProgram(m_meshInfo.back().subsetInfo.back().shadersID);

				//Obtener locaciones de Attributes
				m_meshInfo.back().subsetInfo.back().vertexAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Vertex");
				m_meshInfo.back().subsetInfo.back().normalAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Normal");
				m_meshInfo.back().subsetInfo.back().binormalAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Binormal");
				m_meshInfo.back().subsetInfo.back().tangentAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Tangent");
				m_meshInfo.back().subsetInfo.back().uvAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "UV");

				//Obtener locaciones de Uniforms
				m_meshInfo.back().subsetInfo.back().matWorldViewUniformLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "WV");
				m_meshInfo.back().subsetInfo.back().matWorldViewProjUniformLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "WVP");
				m_meshInfo.back().subsetInfo.back().matWorldUniformLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "World");
				m_meshInfo.back().subsetInfo.back().lightLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "light");
				m_meshInfo.back().subsetInfo.back().lightColLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "lightColor");
				m_meshInfo.back().subsetInfo.back().camPosLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "camPos");
				m_meshInfo.back().subsetInfo.back().specExpLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "specExp");
				m_meshInfo.back().subsetInfo.back().attMaxLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "attMax");
				m_meshInfo.back().subsetInfo.back().camFarLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "camFar");
				
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
			glUniformMatrix4fv(sIt->matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);
			if (sIt->matWorldViewUniformLoc != -1)
				glUniformMatrix4fv(sIt->matWorldViewUniformLoc, 1, GL_FALSE, &WV.m[0][0]);
			if (sIt->matWorldUniformLoc != -1)
				glUniformMatrix4fv(sIt->matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
			if (sIt->lightLoc != -1)
				glUniform3fv(sIt->lightLoc, 1, &pScProp->Lights[0].Position.x);
			if (sIt->lightColLoc != -1)
				glUniform3fv(sIt->lightColLoc, 1, &pScProp->Lights[0].Color.r);
			if (sIt->camPosLoc != -1)
				glUniform3fv(sIt->camPosLoc, 1, &pScProp->pCameras[0]->m_pos.x);
			if (sIt->specExpLoc != -1)
				glUniform1fv(sIt->specExpLoc, 1, &pScProp->specExp);
			if (sIt->attMaxLoc != 1)
				glUniform1fv(sIt->attMaxLoc, 1, &pScProp->attMax);
			if (sIt->camFarLoc != 1)
				glUniform1fv(sIt->camFarLoc, 1, &pScProp->pCameras[0]->farPlane);
			//Enable Attributes
			glEnableVertexAttribArray(sIt->vertexAttribLocs);
			if (sIt->normalAttribLocs != -1)
				glEnableVertexAttribArray(sIt->normalAttribLocs);
			if (sIt->binormalAttribLocs != -1)
				glEnableVertexAttribArray(sIt->binormalAttribLocs);
			if (sIt->tangentAttribLocs != -1)
				glEnableVertexAttribArray(sIt->tangentAttribLocs);
			if (sIt->uvAttribLocs != -1)
				glEnableVertexAttribArray(sIt->uvAttribLocs);
			//Specify Attributes location
			glVertexAttribPointer(sIt->vertexAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(0));
			if (sIt->normalAttribLocs != -1)
			{
				glVertexAttribPointer(sIt->normalAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(16));
			}
			if (sIt->tangentAttribLocs != -1)
			{
				glVertexAttribPointer(sIt->tangentAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(32));
			}
			if (sIt->binormalAttribLocs != -1)
			{
				glVertexAttribPointer(sIt->binormalAttribLocs, 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(48));
			}
			if (sIt->uvAttribLocs != -1)
			{
				glVertexAttribPointer(sIt->uvAttribLocs, 2, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(64));
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

			if (sIt->normalAttribLocs != -1) {
				glDisableVertexAttribArray(sIt->normalAttribLocs);
			}
			if (sIt->uvAttribLocs != -1) {
				glDisableVertexAttribArray(sIt->uvAttribLocs);
			}
			glDisableVertexAttribArray(sIt->vertexAttribLocs);
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
void ModelGL::SetShaderBySignature(unsigned long sig)
{
	for (auto &meshIt : m_meshInfo)
	{
		for (auto &subsetIt : meshIt.subsetInfo)
		{
			Shader* actual = subsetIt.m_shader;
			subsetIt.m_shader = (GLShader*)ShaderManager::GetShaderBySignature(subsetIt.sig);
			if (subsetIt.m_shader != actual)
			{
				subsetIt.shadersID = ((GLShader*)subsetIt.m_shader)->ShaderID;
				glLinkProgram(subsetIt.shadersID);
				glUseProgram(subsetIt.shadersID);

				//Obtener locaciones de Attributes
				subsetIt.vertexAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Vertex");
				subsetIt.normalAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Normal");
				subsetIt.binormalAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Binormal");
				subsetIt.tangentAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Tangent");
				subsetIt.uvAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "UV");

				//Obtener locaciones de Uniforms
				subsetIt.matWorldViewUniformLoc = glGetUniformLocation(subsetIt.shadersID, "WV");
				subsetIt.matWorldViewProjUniformLoc = glGetUniformLocation(subsetIt.shadersID, "WVP");
				subsetIt.matWorldUniformLoc = glGetUniformLocation(subsetIt.shadersID, "World");
				subsetIt.lightLoc = glGetUniformLocation(subsetIt.shadersID, "light");
				subsetIt.lightColLoc = glGetUniformLocation(subsetIt.shadersID, "lightColor");
				subsetIt.camPosLoc = glGetUniformLocation(subsetIt.shadersID, "camPos");
				subsetIt.specExpLoc = glGetUniformLocation(subsetIt.shadersID, "specExp");
				subsetIt.attMaxLoc = glGetUniformLocation(subsetIt.shadersID, "attMax");
				subsetIt.camFarLoc = glGetUniformLocation(subsetIt.shadersID, "camFar");
			}

		}
	}
}
ModelGL::~ModelGL()
{
}
#endif
