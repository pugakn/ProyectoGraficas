#pragma once
#include <string>
#define TYPE_FRAME 1
#define TYPE_MESH 2
#define TYPE_TEMPLATE 3
#define TYPE_MESH_NORMALS 4
#define TYPE_MESH_TEXT_CORDS 5
struct vertexStruct {
	float x, y, z, w;
	float nx, ny, nz, nw;
	float s, t;
};
class MeshParser
{
private:
	unsigned int m_pointer = 0;
	unsigned int m_meshCount = 0;
	char * fileBuffer;
	size_t fileSize;

	std::string getName();
	int getType(int tempPointer);
	void getMeshPositions();
	void getMeshIndices();
	void getMeshNormals();
	void getMeshTextureCords();
	//std::string SearchElement(char condition);
public:
	vertexStruct* m_vbo;
	unsigned short*	m_indexBuffer;
	int m_vertexSize;
	int m_indexSize;

	bool LoadFile(const char* fileName);
	void ReadFile();
	void Deallocate();
	MeshParser() {};
	~MeshParser() { Deallocate(); };
};

