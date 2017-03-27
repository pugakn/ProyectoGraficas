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
	lightColor = Vector3D(1, 0.2, 0.2);
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
	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);
	delete[] vsSourceP;
	delete[] fsSourceP;


	//Iterar cada Mesh y subsets
	for (auto &meshIt: parser.m_meshes)
	{		
		for (auto &subsetIt : meshIt.m_subsets)
		{
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


			if (subsetIt.m_effects.m_specularMap != "")
				Defines += "#define USE_SPEC_MAP\n\n";
			if (subsetIt.m_effects.m_glossMap != "")
				Defines += "#define USE_GLOSS_MAP\n\n";
			if (subsetIt.m_effects.m_normalMap != "")
				Defines += "#define USE_NORMAL_MAP\n\n";

			Defines += "#define USE_PIXELLIGHTING \n\n";
			Defines += "#define USING_ATENUATION \n\n";
			Defines += "#define USE_SPECULAR_BLIN \n\n";



			vstr = Defines + vstr;
			fstr = Defines + fstr;

			GLuint vshader_id = createShader(GL_VERTEX_SHADER, const_cast<char*>(vstr.c_str()));
			GLuint fshader_id = createShader(GL_FRAGMENT_SHADER, const_cast<char*>(fstr.c_str()));

			//Attach Shaders
			shadersID.push_back(glCreateProgram());
			glAttachShader(shadersID.back(), vshader_id);
			glAttachShader(shadersID.back(), fshader_id);

			glLinkProgram(shadersID.back());
			glUseProgram(shadersID.back());

			//Obtener locaciones de Attributes
			vertexAttribLocs.push_back(glGetAttribLocation(shadersID.back(), "Vertex"));
			normalAttribLocs.push_back(glGetAttribLocation(shadersID.back(), "Normal"));
			binormalAttribLocs.push_back(glGetAttribLocation(shadersID.back(), "Binormal"));
			tangentAttribLocs.push_back(glGetAttribLocation(shadersID.back(), "Tangent"));
			uvAttribLocs.push_back(glGetAttribLocation(shadersID.back(), "UV"));

			//Obtener locaciones de Uniforms
			matWorldViewProjUniformLoc = glGetUniformLocation(shadersID.back(), "WVP");
			matWorldUniformLoc = glGetUniformLocation(shadersID.back(), "World");
			lightLoc = glGetUniformLocation(shadersID.back(), "light");
			lightColLoc = glGetUniformLocation(shadersID.back(), "lightColor");
			camPosLoc = glGetUniformLocation(shadersID.back(), "camPos");
			specExpLoc = glGetUniformLocation(shadersID.back(), "specExp");
			attMaxLoc = glGetUniformLocation(shadersID.back(), "attMax");
			//Cargar Texturas
			textureInfo.push_back(TextureInfo());
			bool DifFound = false;
			for (std::size_t f = 0; f<Textures.size(); f++) {
				Texture *ttex = Textures[f];
				std::string ttstr = std::string(ttex->optname);
				if (ttstr == subsetIt.m_effects.m_difusePath.c_str()) {
					textureInfo.back().IdsTex.push_back(ttex->id);
					textureInfo.back().IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "diffuse"));
					DifFound = true;
					break;
				}
			}
			if (!DifFound) {
				Texture *tex = new TextureGL;
				int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_difusePath.c_str()));
				if (textureID != -1) {
					Textures.push_back(tex);
					textureInfo.back().IdsTex.push_back(textureID);
					textureInfo.back().IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "diffuse"));
				}
				else {
					std::cout << "Texture not Found" << std::endl;
					delete tex;
				}
			}
			if (subsetIt.m_effects.m_specularMap != "") {
				bool SpecFound = false;
				for (std::size_t f = 0; f<SpecularTextures.size(); f++) {
					Texture *ttex = SpecularTextures[f];
					std::string ttstr = std::string(ttex->optname);
					if (ttstr == subsetIt.m_effects.m_specularMap.c_str()) {
						textureInfo.back().IdsTex.push_back(ttex->id);
						textureInfo.back().IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "specularMap"));
						SpecFound = true;
						break;
					}
				}
				if (!SpecFound) {
					Texture *tex = new TextureGL;
					int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_specularMap.c_str()));
					if (textureID != -1) {
						SpecularTextures.push_back(tex);
						textureInfo.back().IdsTex.push_back(textureID);
						textureInfo.back().IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "specularMap"));
					}
					else {
						std::cout << "Texture not Found" << std::endl;
						delete tex;
					}
				}
			}
			if (subsetIt.m_effects.m_glossMap != "") {
				bool GlossFound = false;
				for (std::size_t f = 0; f<Textures.size(); f++) {
					Texture *ttex = Textures[f];
					std::string ttstr = std::string(ttex->optname);
					if (ttstr == subsetIt.m_effects.m_glossMap.c_str()) {
						textureInfo.back().IdsTex.push_back(ttex->id);
						textureInfo.back().IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "glossMap"));
						GlossFound = true;
						break;
					}
				}
				if (!GlossFound) {
					Texture *tex = new TextureGL;
					int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_glossMap.c_str()));
					if (textureID != -1) {
						Textures.push_back(tex);
						textureInfo.back().IdsTex.push_back(textureID);
						textureInfo.back().IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "glossMap"));
					}
					else {
						std::cout << "Texture not Found" << std::endl;
						delete tex;
					}
				}
			}
			if (subsetIt.m_effects.m_normalMap != "") {
				bool NormalFound = false;
				for (std::size_t f = 0; f<Textures.size(); f++) {
					Texture *ttex = Textures[f];
					std::string ttstr = std::string(ttex->optname);
					if (ttstr == subsetIt.m_effects.m_normalMap.c_str()) {
						textureInfo.back().IdsTex.push_back(ttex->id);
						textureInfo.back().IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "normalMap"));
						NormalFound = true;
						break;
					}
				}
				if (!NormalFound) {
					Texture *tex = new TextureGL;
					int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_normalMap.c_str()));
					if (textureID != -1) {
						Textures.push_back(tex);
						textureInfo.back().IdsTex.push_back(textureID);
						textureInfo.back().IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "normalMap"));
					}
					else {
						std::cout << "Texture not Found" << std::endl;
						delete tex;
					}
				}
			}

			//Generar buffer de Indices
			IBs.push_back(0);//
			glGenBuffers(1, &IBs.back()); 
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBs.back());
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

	//Vector3D light = Vector3D(l);
	//------------------------------------------------------------------//
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	size_t index = 0;
	for (size_t i = 0; i < parser.m_meshes.size(); i++)
	{
		//Set actual shader
		glUseProgram(shadersID[i]);
		//Set Uniforms
		glUniformMatrix4fv(matWorldUniformLoc, 1, GL_FALSE, &transform.m[0][0]);
		glUniformMatrix4fv(matWorldViewProjUniformLoc, 1, GL_FALSE, &WVP.m[0][0]);
		if (lightLoc != -1)
			glUniform3fv(lightLoc,1, &pScProp->Lights[0].Position.x);
		if (lightColLoc != -1)
			glUniform3fv(lightColLoc, 1, &pScProp->Lights[0].Color.r);
		if (camPosLoc != -1)
			glUniform3fv(camPosLoc, 1, &pScProp->pCameras[0]->m_pos.x);
		if (specExpLoc != -1)
			glUniform1fv(specExpLoc, 1, &pScProp->specExp);
		if (attMaxLoc != 1)
			glUniform1fv(attMaxLoc, 1, &pScProp->attMax);
		//Enable Attributes
		glEnableVertexAttribArray(vertexAttribLocs[i]);
		if (normalAttribLocs[i] != -1)
			glEnableVertexAttribArray(normalAttribLocs[i]);
		if (binormalAttribLocs[i] != -1)
			glEnableVertexAttribArray(binormalAttribLocs[i]);
		if (tangentAttribLocs[i] != -1)
			glEnableVertexAttribArray(tangentAttribLocs[i]);
		if (uvAttribLocs[i] != -1)
			glEnableVertexAttribArray(uvAttribLocs[i]);
		//Specify Attributes location
		glVertexAttribPointer(vertexAttribLocs[i], 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(0));
		if (normalAttribLocs[i] != -1)
		{
			glVertexAttribPointer(normalAttribLocs[i], 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(16));
		}
		if (tangentAttribLocs[i] != -1)
		{
			glVertexAttribPointer(tangentAttribLocs[i], 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(32));
		}
		if (binormalAttribLocs[i] != -1)
		{
			glVertexAttribPointer(binormalAttribLocs[i], 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(48));
		}
		if (uvAttribLocs[i] != -1)
		{
			glVertexAttribPointer(uvAttribLocs[i], 2, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(64));
		}
		for (size_t j = 0; j < parser.m_meshes[i].m_subsets.size(); j++)
		{
			//Bind Buffers
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBs[index]);
			int active = 0;
			for (size_t k = 0; k < textureInfo[index].IdsTex.size(); k++)
			{
				glActiveTexture(GL_TEXTURE0 + active);//Set Active texture unit
				glBindTexture(GL_TEXTURE_2D, textureInfo[index].IdsTex[k]);
				glUniform1i(textureInfo[index].IdTexUniformLocs[k], active); //Specify location
				active++;
			}

#if USING_32BIT_IB
			glDrawElements(GL_TRIANGLES, parser.m_meshes[i].m_subsets[j].m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
#else
			glDrawElements(GL_TRIANGLES, parser.m_meshes[i].m_subsets[j].m_indexBuffer.size(), GL_UNSIGNED_SHORT, 0);
#endif // USING_32BIT_IB

			index++;

		}

		//Disable Shader
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(vertexAttribLocs[i]);
		if (normalAttribLocs[i] != -1) {
			glDisableVertexAttribArray(normalAttribLocs[i]);
		}
		if (uvAttribLocs[i] != -1) {
			glDisableVertexAttribArray(uvAttribLocs[i]);
		}
		glUseProgram(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//----------------------------------------------------------------//
}

void ModelGL::Destroy()
{
}
ModelGL::~ModelGL()
{
}
#endif
