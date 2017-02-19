#include "MeshParser.h"
#include <boost/iostreams/device/mapped_file.hpp>

#define USING_OPENGL
bool MeshParser::LoadFile(const char* fileName)
{
	boost::iostreams::mapped_file_source file;
	file.open(fileName);
	if (file.is_open()) {
		m_pointer = const_cast<char*>(file.data());
		fileSize = file.size();
		ReadFile();
		file.close();
	}
	if (m_meshCount != 0) return true;
	return false;

	
	//FILE * pFile;
	//pFile = fopen(fileName, "r");
	//if (pFile == NULL)
	//{
	//	printf( "ERROR AL ABRIR EL ARCHIVO\n" );
	//	return false;
	//}

	//// Obtain file size
	//fseek(pFile, 0, SEEK_END);
	//fileSize = ftell(pFile);
	//rewind(pFile);

	 //Allocate memory
	//fileBuffer = new char[fileSize];

	// Copy the file into the buffer
	/*fread(fileBuffer, 1, fileSize, pFile);
	fclose(pFile);
	m_pointer = &fileBuffer[0];
	ReadFile();
	delete[] fileBuffer;

	if (m_meshCount != 0) return true;
	return false;*/
}

void MeshParser::ReadFile()
{
	m_vertexSize = 0;
	m_meshCount = 0;
	indexPos = 0;
	vertexPos = 0;
	normalPos = 0;
	textCordsPos = 0;

	std::string name;
	int type;
	//Leer Mesh
	//TODO: Soportar más de una Mesh
	char* bufferEnd = &m_pointer[fileSize - 1];
	m_pointer = strstr(m_pointer,"Mesh ");
	m_pointer = m_pointer + 64;
	m_pointer = strstr(m_pointer, "Mesh ");
	while (m_pointer != bufferEnd)
	{
		if (*m_pointer == '{')
		{
			name = getName();
			type = getType(m_pointer - 3 - name.length());
			switch (type)
			{
				case TYPE_MESH:
					m_meshes.push_back(xMesh());
					getMeshPositions();
					getMeshIndices();
					++m_meshCount;
					break;
				case TYPE_MESH_NORMALS:
					getMeshNormals();
					break;
				case TYPE_MESH_TEXT_CORDS:
					getMeshTextureCords();
					//return;//
					break;
				case TYPE_MESH_MATERIAL_LIST:
					createSubsetts();
					getMaterials();
					break;
			}
		}
		++m_pointer;
	}
}

std::string MeshParser::getName()
{
	char* tempPointer = m_pointer - 2;
	std::string name;
	while (true)
	{
		if (*tempPointer == ' ')
			break;
		name.push_back(*tempPointer);
		tempPointer--;
	}
	return name;
}

int MeshParser::getType(char* tempPointer)
{
	std::string type;
	while (true)
	{
		if (*tempPointer == '\n' || *tempPointer == ' ')
			break;
		type.push_back(*tempPointer);
		tempPointer--;
	}
	if (type == "emarF")
		return TYPE_FRAME;
	if (type == "hseM")
		return TYPE_MESH;
	if (type == "etalpmet")
		return TYPE_TEMPLATE;
	if (type == "slamroNhseM")
		return TYPE_MESH_NORMALS;
	if (type == "sdrooCerutxeThseM")
		return TYPE_MESH_TEXT_CORDS;
	if (type == "tsiLlairetaMhseM")
		return TYPE_MESH_MATERIAL_LIST;
	if (type == "lairetaM")
		return TYPE_MESH_MATERIAL;

	return 0;
}

void MeshParser::getMeshPositions()
{
	++m_pointer;
	std::string numString;
	while (!(*m_pointer == ';'))
	{
		numString.push_back(*m_pointer);
		++m_pointer;
	}
	++m_pointer;
	int nPositions = std::stoi(numString);
	numString.clear();
	offset = m_vertexSize;
	m_vertexSize += nPositions;
	//m_vbo = new vertexStruct[nPositions];
	m_vbo.resize(m_vertexSize);
	for (; vertexPos < m_vertexSize; vertexPos++)
	{
		m_pointer += 2;
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		//m_vbo[i].x = std::stof(numString);
		m_vbo[vertexPos].x = std::stof(numString);
		numString.clear();
		while (!(*m_pointer == ';'))
		{

			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[vertexPos].y = std::stof(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[vertexPos].z = std::stof(numString);
		m_vbo[vertexPos].w = 1;
		numString.clear();
		m_pointer += 2;
	}

	//vertexPos++;
}

void MeshParser::getMeshIndices()
{
	++m_pointer;
	std::string numString;
	while (!(*m_pointer == ';'))
	{
		numString.push_back(*m_pointer);
		++m_pointer;
	}
	++m_pointer;
	int nIndices = std::stoi(numString);
	numString.clear();
	//m_indexSize += nIndices;
	m_meshes.back().m_indexBuffer.resize(nIndices*3);
	//m_indexBuffer = new unsigned short[nIndices * 3];
	for (int i=0; i < nIndices *3; i +=3)
	{
		//m_pointer += 5;
		while (!(*m_pointer++ == ';'));
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
#ifdef USING_OPENGL
		m_meshes.back().m_indexBuffer[i + 2] = std::stoi(numString) + offset;
#else
		m_indexBuffer[i] = std::stoi(numString);
#endif // USING_OPENGL
		numString.clear();
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_indexBuffer[i + 1] = std::stoi(numString) + offset;
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
#ifdef USING_OPENGL
		m_meshes.back().m_indexBuffer[i] = std::stoi(numString) + offset;
#else
		m_indexBuffer[i + 2] = std::stoi(numString);
#endif // USING_OPENGL
		numString.clear();
		++m_pointer;
	}
	//indexPos++;
}

void MeshParser::getMeshNormals()
{
	++m_pointer;
	std::string numString;
	while (!(*m_pointer == ';'))
	{
		numString.push_back(*m_pointer);
		++m_pointer;
	}
	++m_pointer;
	int nNormals = std::stoi(numString);
	numString.clear();
	for (; normalPos < m_vertexSize; normalPos++)
	{
		m_pointer += 2;
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[normalPos].nx = std::stof(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[normalPos].ny = std::stof(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[normalPos].nz = std::stof(numString);
		m_vbo[normalPos].nw = 1;
		numString.clear();
		m_pointer += 2;
	}
}
void MeshParser::getMeshTextureCords()
{
	++m_pointer;
	std::string numString;
	while (!(*m_pointer == ';'))
	{
		numString.push_back(*m_pointer);
		++m_pointer;
	}
	++m_pointer;
	int nTextCords = std::stoi(numString);
	numString.clear();
	for (; textCordsPos < m_vertexSize; textCordsPos++)//nTextCords
	{
		++m_pointer;
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[textCordsPos].s = std::stof(numString);
		numString.clear();
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[textCordsPos].t = std::stof(numString);
		numString.clear();
		++m_pointer;
	}
}
void MeshParser::createSubsetts()
{
	++m_pointer;
	std::string numString;
	while (!(*m_pointer == ';'))
	{
		numString.push_back(*m_pointer);
		++m_pointer;
	}
	++m_pointer;
	int nMaterials = std::stoi(numString);
	numString.clear();
	m_meshes.back().m_subsets.resize(nMaterials);

	while (!(*m_pointer == ';'))
	{
		numString.push_back(*m_pointer);
		++m_pointer;
	}
	++m_pointer;
	int nTriangles = std::stoi(numString);
	numString.clear();
	for (int i = 0; i < nTriangles; i++)
	{
		//while (!(*m_pointer++ == ';'));
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		int index = i * 3;
		m_meshes.back().m_subsets[std::stoi(numString)].m_indexBuffer.push_back(m_meshes.back().m_indexBuffer[index]);
		m_meshes.back().m_subsets[std::stoi(numString)].m_indexBuffer.push_back(m_meshes.back().m_indexBuffer[index +1]);
		m_meshes.back().m_subsets[std::stoi(numString)].m_indexBuffer.push_back(m_meshes.back().m_indexBuffer[index +2]);

		numString.clear();
	}

}
void MeshParser::getMaterials()
{
	for (int i = 0; i < m_meshes.back().m_subsets.size(); i++)
	{
		m_pointer = strstr(m_pointer, "TextureFilename"); //EffectParamString "diffuseMap";
		while (!(*m_pointer++ == '\\'));
		m_pointer++;
		std::string pathString;
		while (!(*m_pointer == '"'))
		{
			pathString.push_back(*m_pointer);
			++m_pointer;
		}
		m_pointer += 2;
		m_meshes.back().m_subsets[i].m_effects.m_difusePath = pathString;
	}
}
void MeshParser::Deallocate()
{
	m_vbo.clear();
}
//std::string MeshParser::SearchElement(char condition)
//{
//	std::string elementString;
//	while (true)
//	{
//		if (fileBuffer[m_pointer] == condition)
//		{
//			++m_pointer;
//			break;
//		}
//		elementString.push_back(fileBuffer[m_pointer]);
//		++m_pointer;
//	}
//	return elementString;
//}
