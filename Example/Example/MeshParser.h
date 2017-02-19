#pragma once
#include <string>
#include <vector>
#define TYPE_FRAME 1
#define TYPE_MESH 2
#define TYPE_TEMPLATE 3
#define TYPE_MESH_NORMALS 4
#define TYPE_MESH_TEXT_CORDS 5
#define TYPE_MESH_MATERIAL_LIST 6
#define TYPE_MESH_MATERIAL 7

namespace xf {
	namespace attributes {
		enum E
		{
			HAS_POSITION = 1,
			HAS_NORMAL = HAS_POSITION << 1,
			HAS_TANGENT = HAS_POSITION << 2,
			HAS_BINORMAL = HAS_POSITION << 3,
			HAS_TEXCOORD0 = HAS_POSITION << 4,
			HAS_TEXCOORD1 = HAS_POSITION << 5,
			HAS_TEXCOORD2 = HAS_POSITION << 6,
			HAS_TEXCOORD3 = HAS_POSITION << 7,
			HAS_VERTEXCOLOR = HAS_POSITION << 8,
			HAS_SKINWEIGHTS0 = HAS_POSITION << 9,
			HAS_SKINWEIGHTS1 = HAS_POSITION << 10,
			HAS_SKININDEXES0 = HAS_POSITION << 11,
			HAS_SKININDEXES1 = HAS_POSITION << 12,
			HAS_CUSTOM_DATA_4BYTES = HAS_POSITION << 13,
			HAS_CUSTOM_DATA_8BYTES = HAS_POSITION << 14,
			HAS_CUSTOM_DATA_16BYTES = HAS_POSITION << 15,
		};
	}
}
struct vertexStruct {
	float x, y, z, w;
	float nx, ny, nz, nw;
	float s, t;
};
struct xMeshEffects {
	std::string m_difusePath;
};
struct xMeshSubset {
	std::vector<unsigned short> m_indexBuffer;
	xMeshEffects m_effects;
};
struct xMesh{
	std::vector<unsigned short>	m_indexBuffer;
	std::vector<xMeshSubset> m_subsets;
	unsigned long m_vertexAttributes;
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
	void getMaterials();
	//std::string SearchElement(char condition);
public:
	std::vector<vertexStruct> m_vbo;
	std::vector<unsigned short>	m_indexBuffer;
	std::vector<xMesh> m_meshes;

	int m_vertexSize;
	//size_t indexPos;
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

