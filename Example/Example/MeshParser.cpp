#include "MeshParser.h"
#include <boost/iostreams/device/mapped_file.hpp>

#define USING_OPENGL
bool MeshParser::LoadFile(const char* fileName)
{
	boost::iostreams::mapped_file_source file;
	file.open(fileName);
	if (file.is_open()) {
		m_pointer = const_cast<char*>(file.data());
	}
	ReadFile();
	file.close();
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

	//// Allocate memory
	//fileBuffer = new char[fileSize];

	//// Copy the file into the buffer
	//fread(fileBuffer, 1, fileSize, pFile);
	//fclose(pFile);
	//m_pointer = &fileBuffer[0];
	//ReadFile();
	//delete[] fileBuffer;

	//if (m_meshCount != 0) return true;
	//return false;
}

void MeshParser::ReadFile()
{
	std::string name;
	int type;
	//Leer Mesh
	//TODO: Soportar más de una Mesh
	m_pointer = strstr(m_pointer,"Mesh ");
	m_pointer = m_pointer + 64;
	m_pointer = strstr(m_pointer, "Mesh ");
	while (m_pointer != &fileBuffer[fileSize-1])
	{
		if (*m_pointer == '{')
		{
			name = getName();
			type = getType(m_pointer - 3 - name.length());
			switch (type)
			{
				case TYPE_MESH:
					++m_meshCount;
					getMeshPositions();
					getMeshIndices();
					break;
				case TYPE_MESH_NORMALS:
					getMeshNormals();
					break;
				case TYPE_MESH_TEXT_CORDS:
					getMeshTextureCords();
					return;//
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
	m_vertexSize = nPositions;
	m_vbo = new vertexStruct[nPositions];
	for (int i = 0; i < nPositions; i++)
	{
		m_pointer += 2;
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[i].x = std::stof(numString);
		numString.clear();
		while (!(*m_pointer == ';'))
		{

			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[i].y = std::stof(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[i].z = std::stof(numString);
		m_vbo[i].w = 1;
		numString.clear();
		m_pointer += 2;
	}
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
	m_indexSize = nIndices;
	m_indexBuffer = new unsigned short[nIndices * 3];
	for (int i = 0; i < nIndices*3; i+=3)
	{
		m_pointer += 6;

		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
#ifdef USING_OPENGL
		m_indexBuffer[i + 2] = std::stoi(numString);
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
		m_indexBuffer[i + 1] = std::stoi(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
#ifdef USING_OPENGL
		m_indexBuffer[i] = std::stoi(numString);
#else
		m_indexBuffer[i + 2] = std::stoi(numString);
#endif // USING_OPENGL
		numString.clear();
		++m_pointer;
	}
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
	m_vertexSize = nNormals;
	for (int i = 0; i < nNormals; i++)
	{
		m_pointer += 2;
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[i].nx = std::stof(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[i].ny = std::stof(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[i].nz = std::stof(numString);
		m_vbo[i].nw = 1;
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
	for (int i = 0; i < nTextCords; i++)
	{
		++m_pointer;
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[i].s = std::stof(numString);
		numString.clear();
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_vbo[i].t = std::stof(numString);
		numString.clear();
		++m_pointer;
	}
}
void MeshParser::Deallocate()
{
	delete[] m_vbo;
	delete[] m_indexBuffer;
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
