#pragma once
#include <string>
#include <vector>
#include "Config.h"
#include "Vector3D.h"
#include "Matrix4D.h"
#include "Animation.h"

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
			HAS_CUSTOM_DATA_16BYTES = HAS_POSITION << 15
		};
	}
}
struct vertexStruct {
	float x, y, z, w;
	float nx, ny, nz, nw;
	float tx, ty, tz, tw;
	float bx, by, bz, bw;
	float s, t;
	int wIndex[4];
	float wWeight[4];
	vertexStruct()
	{
		wIndex[0] = -1;
		wIndex[1] = -1;
		wIndex[2] = -1;
		wIndex[3] = -1;
	}
};

struct xBoneWeightInfo
{
	std::vector<int> boneIndex;
	std::vector<float> weight;
};
struct xMeshEffects {
	std::string m_difusePath;
	std::string m_specularMap;
	std::string m_normalMap;
	std::string m_glossMap;
	bool m_noLighting;
	bool m_blFlipGreenChanel;
	float m_glossines;
	float m_specLevel;
};
struct xSkinWeigths {
	std::vector<unsigned short> boneIndex;
	std::vector<float> weight;
	Matrix4D spaceTransformMatrix;
};

struct xMeshSubset {
#if USING_32BIT_IB
	std::vector<unsigned int> m_indexBuffer;
#else
	std::vector<unsigned short> m_indexBuffer;
#endif // USING_32BIT_IB
	xMeshEffects m_effects;
};
struct xMesh{
#if USING_32BIT_IB
	std::vector<unsigned int> m_indexBuffer;
#else
	std::vector<unsigned short> m_indexBuffer;
#endif // USING_32BIT_IB
	std::vector<xMeshSubset> m_subsets;
	unsigned long m_vertexAttributes;
	std::vector<xSkinWeigths> m_skinWeights;
	std::vector<Matrix4D> m_skinWeightsOffset;
	std::vector<vertexStruct> m_vbo;
};


struct xBone {
	std::string name;
	Matrix4D bone;
	std::vector<int> child;
	std::vector<int> brothers;
	int dad;
};



class MeshParser
{
private:
	enum
	{
		TYPE_FRAME = 1,
		TYPE_MESH ,
		TYPE_TEMPLATE ,
		TYPE_MESH_NORMALS ,
		TYPE_MESH_TEXT_CORDS ,
		TYPE_MESH_MATERIAL_LIST ,
		TYPE_MESH_MATERIAL ,
		TYPE_MESH_DECL_DATA ,
		TYPE_MESH_BONE_WIGHTS ,
		TYPE_ANIM_TICKS_PERS_ECOND,
		TYPE_ANIMATION_SET
	};
	char* m_pointer;
	unsigned int m_meshCount = 0;
	size_t fileSize;
	std::string m_ActualName;
	int m_ActualType;

	std::string getName();
	int getType(char* tempPointer);
	void getMeshPositions();
	void getMeshIndices();
	void getMeshNormals();
	void getMeshTextureCords();
	void createSubsetts();
	void getMaterials();
	void getDeclData();
	void getBones();
	void InsertBonesSiblingsOnEachBone();
	Matrix4D getFrameTransformMatrix();
	void ignoreObjectMatrixComment();
	bool IsNextACloseBlock();

	void LoadWeights();
	std::string LoadBoneName();
	int LoadNumWeights();
	std::vector<int> LoadWeightsIndex(int numWeights, int actualBoneIndex);
	void LoadIndexWeights(int numWeights, std::vector<int>& index);
	Matrix4D LoadSpaceTransformMatrix();

	void ReadAnimationTicksPerSecond();
	void ReadAnimationSet();
	void ReadAnimationRotations();
	void ReadAnimationPositions();
	void ReadAnimationScales();

	int ReadAnimTime();
	float readAnimFloat();

	int actualDad = -1;
	int openBlocks = 0;
	int actualBone = 0;
	int indexPos;
public:
	std::vector<xMesh> m_meshes;

	std::vector<xBone> bones;
	//std::vector<Matrix4D> m_skinWeightsOffset;//
	std::vector<xAnimationSet> animationSets;
	bool LoadFile(const char* fileName);
	void ReadFile();
	void Deallocate();
	MeshParser() {};
	~MeshParser() { Deallocate(); };
};

