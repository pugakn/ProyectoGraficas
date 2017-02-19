#pragma once
#include <string>
#include <vector>
#define TYPE_FRAME 1
#define TYPE_MESH 2
#define TYPE_TEMPLATE 3
#define TYPE_MESH_NORMALS 4
#define TYPE_MESH_TEXT_CORDS 5
#define TYPE_MESH_MATERIAL_LIST 6
struct vertexStruct {
	float x, y, z, w;
	float nx, ny, nz, nw;
	float s, t;
};
struct xMesh{
	std::vector<std::vector<unsigned short>> m_subsetIndex;
	std::vector<unsigned short>	m_indexBuffer;
};
class MeshParser
{
private:
	char* m_pointer;
	unsigned int m_meshCount = 0;
	size_t fileSize;

	std::string getName();
	int getType(char* tempPointer);
	void getMeshPositions();
	void getMeshIndices();
	void getMeshNormals();
	void getMeshTextureCords();
	void createSubsetts();
	//std::string SearchElement(char condition);
public:
	//vertexStruct* m_vbo;
	std::vector<vertexStruct> m_vbo;
	//unsigned short*	m_indexBuffer;
	std::vector<unsigned short>	m_indexBuffer;
	std::vector<xMesh> m_meshes;

	int m_vertexSize;
	size_t indexPos;
	size_t vertexPos;
	size_t normalPos;
	size_t textCordsPos;

	size_t offset;

	bool LoadFile(const char* fileName);
	void ReadFile();
	void Deallocate();
	MeshParser() {};
	~MeshParser() { Deallocate(); };
};

