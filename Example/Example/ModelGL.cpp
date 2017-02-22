#include "ModelGL.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include "Timer.h"
#include <iostream>
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
	char *vsSourceP = file2string("VS_Mesh.glsl");
	char *fsSourceP = file2string("FS_Mesh.glsl");
	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);
	delete[] vsSourceP;
	delete[] fsSourceP;

	//Crear shaders
	//Iterar cada Mesh y subsets
	for (auto &meshIt: parser.m_meshes)
	{
		std::string Defines = "";
		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_NORMAL)
			Defines += "#define USE_NORMALS\n\n";
		if (meshIt.m_vertexAttributes&xf::attributes::E::HAS_TEXCOORD0)
			Defines += "#define USE_TEXCOORD0\n\n";

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
		uvAttribLocs.push_back(glGetAttribLocation(shadersID.back(), "UV"));
		//Obtener locaciones de Uniforms
		matWorldViewProjUniformLoc = glGetUniformLocation(shadersID.back(), "WVP");
		matWorldUniformLoc = glGetUniformLocation(shadersID.back(), "World");
		for (auto &subsetIt : meshIt.m_subsets)
		{
			//Cargar Texturas
			bool found = false;
			for (std::size_t f = 0; f<Textures.size(); f++) {
				Texture *ttex = Textures[f];
				std::string ttstr = std::string(ttex->optname);
				if (ttstr == subsetIt.m_effects.m_difusePath.c_str()) {
					IdsTex.push_back(ttex->id);
					IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "diffuse"));
					found = true;
					break;
				}
			}
			if (!found) {
				Texture *tex = new TextureGL;
				int textureID = tex->LoadTexture(const_cast<char*>(subsetIt.m_effects.m_difusePath.c_str()));
				if (textureID != -1) {
					Textures.push_back(tex);
					IdsTex.push_back(textureID);
					IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "diffuse"));
				}
				else {
					std::cout << "Texture not Found" << std::endl;
					IdsTex.push_back(NULL);//
					IdTexUniformLocs.push_back(glGetUniformLocation(shadersID.back(), "diffuse"));//
					delete tex;
				}
			}
			//Generar buffer de Indices
			IBs.push_back(0);//
			glGenBuffers(1, &IBs.back()); //ERROR SUBSETS VARIABLES
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

void ModelGL::Draw(float *t, float *vp)
{

	if (t)
		transform = t;

	Matrix4D VP = Matrix4D(vp);
	Matrix4D WVP = transform*VP;

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
		//Enable Attributes
		glEnableVertexAttribArray(vertexAttribLocs[i]);
		if (normalAttribLocs[i] != -1)
			glEnableVertexAttribArray(normalAttribLocs[i]);
		if (uvAttribLocs[i] != -1)
			glEnableVertexAttribArray(uvAttribLocs[i]);
		//Specify Attributes location
		glVertexAttribPointer(vertexAttribLocs[i], 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(0));
		if (normalAttribLocs[i] != -1)
			glVertexAttribPointer(normalAttribLocs[i], 4, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(16));
		if (uvAttribLocs[i] != -1)
			glVertexAttribPointer(uvAttribLocs[i], 2, GL_FLOAT, GL_FALSE, sizeof(vertexStruct), BUFFER_OFFSET(32));
		for (size_t j = 0; j < parser.m_meshes[i].m_subsets.size(); j++)
		{
			//Bind Buffers
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBs[index]);
			//Specify Texture location
			glActiveTexture(GL_TEXTURE0);//Set Active texture unit
			glBindTexture(GL_TEXTURE_2D, IdsTex[index]);
			glUniform1i(IdTexUniformLocs[index], 0); //Specify location

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
