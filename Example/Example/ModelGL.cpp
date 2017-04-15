#include "ModelGL.h"
#ifdef USING_OPENGL_ES
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include "Timer.h"
#include <iostream>
#include "TextureGL.h"


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
	char *vsSourceP = file2string("Shaders/VS_MeshPL.glsl");
	char *fsSourceP = file2string("Shaders/FS_MeshPL.glsl");
	std::string vstr;
	std::string fstr;
	if (vsSourceP && fsSourceP)
	{
		vstr = std::string(vsSourceP);
		fstr = std::string(fsSourceP);
	}

	delete[] vsSourceP;
	delete[] fsSourceP;


	//Iterar cada Mesh y subsets
	for (auto &meshIt: parser.m_meshes)
	{	
		m_meshInfo.push_back(MeshInfo());
		for (auto &subsetIt : meshIt.m_subsets)
		{
			m_meshInfo.back().subsetInfo.push_back(SubsetInfo());
			//Crear shaders
			std::string Defines = "";
			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_NORMAL)
				Defines += "#define USE_NORMALS\n\n";
			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD0)
				Defines += "#define USE_TEXCOORD0\n\n";
			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD1)
				Defines += "#define USE_TEXCOORD1\n\n";
			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TANGENT)
				Defines += "#define USE_TANGENTS\n\n";
			if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_BINORMAL)
				Defines += "#define USE_BINORMALS\n\n";



			//if (subsetIt.m_effects.m_glossMap != "")
			//	Defines += "#define USE_GLOSS_MAP\n\n";
			if (subsetIt.m_effects.m_normalMap != "")
				Defines += "#define USE_NORMAL_MAP\n\n";
			if (subsetIt.m_effects.m_specularMap != "")
				Defines += "#define USE_SPEC_MAP\n\n";

			Defines += "#define USE_PIXELLIGHTING \n\n";
			Defines += "#define USING_ATENUATION \n\n";
			Defines += "#define USE_SPECULAR_BLIN \n\n";

			vstr = Defines + vstr;
			fstr = Defines + fstr;

			GLuint vshader_id = createShader(GL_VERTEX_SHADER, const_cast<char*>(vstr.c_str()));
			GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, const_cast<char*>(fstr.c_str()));
			if (vshader_id == 0 || fshader_id == 0)
			{
				m_meshInfo.back().subsetInfo.back().shadersID = Utils::DefaultShaderID;
				glLinkProgram(m_meshInfo.back().subsetInfo.back().shadersID);
				glUseProgram(m_meshInfo.back().subsetInfo.back().shadersID);

				m_meshInfo.back().subsetInfo.back().vertexAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Vertex");
				m_meshInfo.back().subsetInfo.back().matWorldViewProjUniformLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "WVP");
			}
			else
			{
				//Attach Shaders
				m_meshInfo.back().subsetInfo.back().shadersID = glCreateProgram();
				glAttachShader(m_meshInfo.back().subsetInfo.back().shadersID, vshader_id);
				glAttachShader(m_meshInfo.back().subsetInfo.back().shadersID, fshader_id);

				glLinkProgram(m_meshInfo.back().subsetInfo.back().shadersID);
				glUseProgram(m_meshInfo.back().subsetInfo.back().shadersID);

				//Obtener locaciones de Attributes
				m_meshInfo.back().subsetInfo.back().vertexAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Vertex");
				m_meshInfo.back().subsetInfo.back().normalAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Normal");
				m_meshInfo.back().subsetInfo.back().binormalAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Binormal");
				m_meshInfo.back().subsetInfo.back().tangentAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "Tangent");
				m_meshInfo.back().subsetInfo.back().uvAttribLocs = glGetAttribLocation(m_meshInfo.back().subsetInfo.back().shadersID, "UV");

				//Obtener locaciones de Uniforms
				m_meshInfo.back().subsetInfo.back().matWorldViewProjUniformLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "WVP");
				m_meshInfo.back().subsetInfo.back().matWorldUniformLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "World");
				m_meshInfo.back().subsetInfo.back().lightLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "light");
				m_meshInfo.back().subsetInfo.back().lightColLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "lightColor");
				m_meshInfo.back().subsetInfo.back().camPosLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "camPos");
				m_meshInfo.back().subsetInfo.back().specExpLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "specExp");
				m_meshInfo.back().subsetInfo.back().attMaxLoc = glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "attMax");
				//Cargar Texturas
				//Difuse
				bool found = false;
				m_meshInfo.back().subsetInfo.back().textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "diffuse"));
				for (std::size_t f = 0; f<Textures.size(); f++) {
					Texture *ttex = Textures[f];
					std::string ttstr = std::string(ttex->optname);
					if (ttstr == subsetIt.m_effects.m_difusePath.c_str()) {
						m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(ttex->id);
						found = true;
						break;
					}
				}
				if (!found) {
					Texture *tex = new TextureGL;
					int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_difusePath.c_str()));
					if (textureID != -1) {
						Textures.push_back(tex);
						m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(textureID);
					}
					else {
						std::cout << "Texture not Found" << std::endl;
						m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(Utils::textureChekerID);
						Textures.push_back(Utils::textureCheker);
						delete tex;
					}
				}
				//Specular

				if (subsetIt.m_effects.m_specularMap != "") {
					m_meshInfo.back().subsetInfo.back().textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "specularMap"));
					found = false;
					for (std::size_t f = 0; f< Textures.size(); f++) {
						Texture *ttex = Textures[f];
						std::string ttstr = std::string(ttex->optname);
						if (ttstr == subsetIt.m_effects.m_specularMap.c_str()) {
							m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(ttex->id);
							found = true;
							break;
						}
					}
					if (!found) {
						Texture *tex = new TextureGL;
						int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_specularMap.c_str()));
						if (textureID != -1) {
							Textures.push_back(tex);
							m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(textureID);
						}
						else {
							std::cout << "Texture not Found" << std::endl;
							m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(Utils::textureChekerID);
							Textures.push_back(Utils::textureCheker);
							delete tex;
						}
					}
				}
				//Gloss
				if (subsetIt.m_effects.m_glossMap != "") {
					m_meshInfo.back().subsetInfo.back().textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "glossMap"));
					found = false;
					for (std::size_t f = 0; f<Textures.size(); f++) {
						Texture *ttex = Textures[f];
						std::string ttstr = std::string(ttex->optname);
						if (ttstr == subsetIt.m_effects.m_glossMap.c_str()) {
							m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(ttex->id);
							found = true;
							break;
						}
					}
					if (!found) {
						Texture *tex = new TextureGL;
						int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_glossMap.c_str()));
						if (textureID != -1) {
							Textures.push_back(tex);
							m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(textureID);
						}
						else {
							std::cout << "Texture not Found" << std::endl;
							m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(Utils::textureChekerID);
							Textures.push_back(Utils::textureCheker);
							delete tex;
						}
					}
				}
				//Normal
				if (subsetIt.m_effects.m_normalMap != "") {
					m_meshInfo.back().subsetInfo.back().textInfo.IdTexUniformLocs.push_back(glGetUniformLocation(m_meshInfo.back().subsetInfo.back().shadersID, "normalMap"));
					found = false;
					for (std::size_t f = 0; f<Textures.size(); f++) {
						Texture *ttex = Textures[f];
						std::string ttstr = std::string(ttex->optname);
						if (ttstr == subsetIt.m_effects.m_normalMap.c_str()) {
							m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(ttex->id);
							found = true;
							break;
						}
					}
					if (!found) {
						Texture *tex = new TextureGL;
						int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_normalMap.c_str()));
						if (textureID != -1) {
							Textures.push_back(tex);
							m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(textureID);
						}
						else {
							std::cout << "Texture not Found" << std::endl;
							m_meshInfo.back().subsetInfo.back().textInfo.IdsTex.push_back(Utils::textureChekerID);
							Textures.push_back(Utils::textureCheker);
							delete tex;
						}
					}
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
	wire.ShaderID = Utils::DefaultShaderID;

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
			glUniformMatrix4fv(sIt->matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);
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
ModelGL::~ModelGL()
{
}
#endif
