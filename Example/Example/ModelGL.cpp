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
	animationManager.model = this;

	Timer timer;
	timer.Init();
	if (!parser.LoadFile(m_fileName.c_str()))
	{
		std::cout << "Error: No se pudo cargar el modelo" << std::endl;
		return;
	}
	timer.Update();
	std::cout << "Archivo cargado en: " << timer.GetDTSecs() << " segundos..." << std::endl;
	m_bones.resize(parser.bones.size());
	if (parser.bones.size())
		parser.m_inverseGlobal =  Inverse(parser.bones[0].bone);


	m_bones = parser.bones;
	animationManager.animationSets = parser.animationSets;
	//Iterar cada Mesh y subsets
	idCube = cubetxt.LoadCubeMap("tstcuben.dds");
	for (auto &meshIt: parser.m_meshes)
	{	
		m_meshInfo.push_back(MeshInfo());
		m_meshInfo.back().m_vboOriginal = meshIt.m_vbo;
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


				auto  set = ShaderManager::GetShaderSetBySignature(m_meshInfo.back().subsetInfo.back().sig, "Shaders/VS_MeshPL.glsl", "Shaders/FS_MeshPL.glsl");
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
				int textureID = Tools::LoadTexture(subsetIt.m_effects.m_difusePath.c_str());
				m_meshInfo.back().subsetInfo.back().diffuseText1ID = textureID;
				if (subsetIt.m_effects.m_glossMap != "")
				{
					textureID = Tools::LoadTexture(subsetIt.m_effects.m_glossMap.c_str());
					m_meshInfo.back().subsetInfo.back().GlossText2ID = textureID;
				}
				if (subsetIt.m_effects.m_normalMap != "")
				{
					textureID = Tools::LoadTexture(subsetIt.m_effects.m_normalMap.c_str());
					m_meshInfo.back().subsetInfo.back().NormalText3ID = textureID;
				}
				if (subsetIt.m_effects.m_specularMap != "")
				{
					textureID = Tools::LoadTexture(subsetIt.m_effects.m_specularMap.c_str());
					m_meshInfo.back().subsetInfo.back().SpecularText4ID = textureID;
				}

				m_meshInfo.back().subsetInfo.back().IdCubeLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "skybox");
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
		//Generar buffer de vertices
		glGenBuffers(1, &m_meshInfo.back().VB);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshInfo.back().VB);
		glBufferData(GL_ARRAY_BUFFER, meshIt.m_vbo.size()* sizeof(vertexStruct), &meshIt.m_vbo[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//TST
	//for (auto&it : parser.bones)
	//{
	//	std::cout << it.name <<std::endl;
	//	for (auto&it2 : it.child)
	//	{
	//		int i = it2;
	//		std::cout <<"\t"<< parser.bones[it2].name <<std::endl;
	//	}
	//}
	
	//////////TST
	//---------------------------------------------------------------//

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
	for (auto &mesh : parser.m_meshes)
	{
		fullIndex.insert(fullIndex.end(), mesh.m_indexBuffer.begin(), mesh.m_indexBuffer.end());
	}
	
	wireframe.CreateWireframe( fullIndex);
	glGenBuffers(1, &wire.IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wire.IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, wireframe.m_indexBuffer.size() * sizeof(unsigned short), &(wireframe.m_indexBuffer[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//Liberar Memoria
	//parser.Deallocate();
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
	//----------------------------------------------------------------//
}

inline void ModelGL::DrawMeshes(const Matrix4D & VP, const Matrix4D & WVP)
{
	for (size_t i = 0; i < parser.m_meshes.size(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshInfo[i].VB);
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

			Matrix4D CamWVP = transform * pScProp->LightsWShadow[0].VP;
			if (locs.matLightCamWVPLoc != -1)
				glUniformMatrix4fv(locs.matLightCamWVPLoc, 1, GL_FALSE, &CamWVP.m[0][0]);//CamWVP
			//if (locs.lightLoc != -1)
			//	glUniform3fv(locs.lightLoc, 1, &pScProp->Lights[0].Position.x);
			//if (locs.lightColLoc != -1)
			//	glUniform3fv(locs.lightColLoc, 1, &pScProp->Lights[0].Color.r);
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
			int k = 0;
			if (sIt->diffuseText1ID != -1)
			{
				glActiveTexture(GL_TEXTURE0 + k);//Set Active texture unit
				glBindTexture(GL_TEXTURE_2D, sIt->diffuseText1ID);
				glUniform1i(((GLShader*)sIt->m_shaderSet[m_shaderType])->m_locs.textureLoc01, k); //Specify location
				k++;
			}
			if (sIt->GlossText2ID != -1)
			{
				glActiveTexture(GL_TEXTURE0 + k);
				glBindTexture(GL_TEXTURE_2D, sIt->GlossText2ID);
				glUniform1i(((GLShader*)sIt->m_shaderSet[m_shaderType])->m_locs.textureLoc02, k); 
				k++;
			}
			if (sIt->NormalText3ID != -1)
			{
				glActiveTexture(GL_TEXTURE0 + k);
				glBindTexture(GL_TEXTURE_2D, sIt->NormalText3ID);
				glUniform1i(((GLShader*)sIt->m_shaderSet[m_shaderType])->m_locs.textureLoc03, k); 
				k++;
			}
			if (sIt->SpecularText4ID != -1)
			{
				glActiveTexture(GL_TEXTURE0 + k);
				glBindTexture(GL_TEXTURE_2D, sIt->SpecularText4ID);
				glUniform1i(((GLShader*)sIt->m_shaderSet[m_shaderType])->m_locs.textureLoc04, k);
				k++;
			}
			glActiveTexture(GL_TEXTURE0 + 5);
			glBindTexture(GL_TEXTURE_CUBE_MAP, idCube);
			glUniform1i(sIt->IdCubeLoc, 5);
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
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

inline void ModelGL::DrawWireframe(const Matrix4D & VP, const Matrix4D & WVP)
{
	for (size_t i = 0; i < parser.m_meshes.size(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_meshInfo[i].VB);
		//for (size_t j = 0; j < parser.m_meshes[i].m_subsets.size(); j++)
		//{
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
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//Disable Shader
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
			}
		}
	}
}
void ModelGL::TransformBone(int index, Matrix4D t)
{
	//m_bones = parser.bones;
	CalcCombinedMatrix(index, t);

	for (size_t i = 0; i < parser.m_meshes.size(); i++)
	{
		for (size_t k = 0; k < parser.m_meshes[i].m_vbo.size(); k++) 
		{
			auto vertex = &m_meshInfo[i].m_vboOriginal[k];
			auto vertexNew = &parser.m_meshes[i].m_vbo[k];
			Vector4D pos(vertex->x,vertex->y,vertex->z,1);
			Vector4D vertexPos(0,0,0,1);

			Vector4D normal(vertex->nx, vertex->ny, vertex->nz, 1);
			Vector4D vertexNorm(0, 0, 0, 1);
			Matrix4D F = Identity();
			for (size_t j = 0; j < 4; j++)
			{
				if (vertex->wIndex[j] != -1)
				{
					auto mat = (vertex->wWeight[j] * (parser.m_meshes[i].m_skinWeightsOffset[vertex->wIndex[j]] * m_bones[vertex->wIndex[j]].bone) * parser.m_inverseGlobal);
					vertexPos = vertexPos + pos *mat ;
					vertexNorm = vertexNorm + normal * mat;
				}


			}
			//if (vertex->wIndex[0] == -1)
			//	vertexPos = pos;
			vertexNew->x = vertexPos.x;
			vertexNew->y = vertexPos.y;
			vertexNew->z = vertexPos.z;

			vertexNew->nx = vertexNorm.x;
			vertexNew->ny = vertexNorm.y;
			vertexNew->nz = vertexNorm.z;
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_meshInfo[i].VB);
		glBufferData(GL_ARRAY_BUFFER, parser.m_meshes[i].m_vbo.size() * sizeof(vertexStruct), &parser.m_meshes[i].m_vbo[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER,0);
	}


}
void ModelGL::SetNewTransforms(int index, Matrix4D t) {
	m_bones[index].bone  = m_bones[index].bone*t;
}

void ModelGL::CalcCombinedMatrix(int index, Matrix4D t)
{
	m_bones[index].bone = parser.bones[index].bone * t;
	for (auto &bone : parser.bones[index].child)
	{
		CalcCombinedMatrix(bone, m_bones[index].bone);
	}
}
ModelGL::~ModelGL()
{
}
#endif
