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
}
void MeshParser::ReadFile()
{
	m_meshCount = 0;
	indexPos = 0;

	char* bufferEnd = &m_pointer[fileSize - 1];
	m_pointer = strstr(m_pointer,"Mesh ");
	m_pointer = m_pointer + 64;
	m_pointer = strstr(m_pointer, "Frame ");
	while (m_pointer != bufferEnd)
	{
		m_ActualName.clear();
		m_ActualType = 0;

		m_pointer = strstr(m_pointer,"{");
		if (m_pointer == NULL) return;
		m_ActualName = getName();
		m_ActualType = getType(m_pointer - 3 - m_ActualName.length());
	
		switch (m_ActualType)
		{
			case TYPE_MESH:
				m_meshes.push_back(xMesh());
				//m_meshes.back().m_skinWeightsOffset.push_back(Identity());//
				//m_skinWeightsOffset.push_back(Identity());//
				m_meshes.back().m_vertexAttributes |= xf::attributes::E::HAS_POSITION;
				getMeshPositions();
				getMeshIndices();
				++m_meshCount;
				m_meshes.back().m_skinWeightsOffset.resize(bones.size());//
				for (auto &it : m_meshes.back().m_skinWeightsOffset)
					it = Identity();
				break;
			case TYPE_MESH_NORMALS:
				m_meshes.back().m_vertexAttributes |= xf::attributes::E::HAS_NORMAL;
				getMeshNormals();
				break;
			case TYPE_MESH_TEXT_CORDS:
				m_meshes.back().m_vertexAttributes |= xf::attributes::E::HAS_TEXCOORD0;
				getMeshTextureCords();
				break;
			case TYPE_MESH_MATERIAL_LIST:
				createSubsetts();
				getMaterials();
				//return;//
				break;
			case TYPE_MESH_DECL_DATA:
				m_meshes.back().m_vertexAttributes |= xf::attributes::E::HAS_BINORMAL;
				m_meshes.back().m_vertexAttributes |= xf::attributes::E::HAS_TANGENT;
				getDeclData();
				break;
			case TYPE_FRAME:
				actualDad = -2;
				openBlocks = 0;
				actualBone = 0;
				m_pointer--;
				getBones();
				InsertBonesSiblingsOnEachBone();
				bones[0].dad = 0;
				//m_meshes.back().m_skinWeightsOffset.resize(bones.size());//
				break;
			case TYPE_MESH_BONE_WIGHTS:
				LoadWeights();
				break;
			case TYPE_ANIM_TICKS_PERS_ECOND:
				animationSets.push_back(xAnimationSet());
				ReadAnimationTicksPerSecond();
				break;
			case TYPE_ANIMATION_SET:
				animationSets.back().name = m_ActualName;
				ReadAnimationSet();
				break;

		}
		if (m_pointer == nullptr)
			break;
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
	if (type == "emarF" )
		if ((m_ActualName.find("piB") != std::string::npos))
		//if (!(m_ActualName.find("ordniliC") != std::string::npos))
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
	if (m_ActualName == "ataDlceD")
		return TYPE_MESH_DECL_DATA;
	if (type == "sthgieWnikS")
		return TYPE_MESH_BONE_WIGHTS;
	if (type == "dnoceSrePskciTminA")
		return TYPE_ANIM_TICKS_PERS_ECOND;
	if (type == "teSnoitaminA")
		return TYPE_ANIMATION_SET;
	

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
	//offset = m_vertexSize;
	//m_vertexSize += nPositions;
	m_meshes.back().m_vbo.resize(nPositions);
	//boneWeightInfo.resize(m_vertexSize);//
	for (int vertexPos = 0; vertexPos < nPositions; vertexPos++)
	{
		m_pointer += 2;
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_vbo[vertexPos].x = std::stof(numString);
		numString.clear();
		while (!(*m_pointer == ';'))
		{

			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_vbo[vertexPos].y = std::stof(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_vbo[vertexPos].z = std::stof(numString);
		m_meshes.back().m_vbo[vertexPos].w = 1;
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
		m_meshes.back().m_indexBuffer[i + 2] = std::stoi(numString) ;
#else
		m_meshes.back().m_indexBuffer[i] = std::stoi(numString) ;
#endif // USING_OPENGL
		numString.clear();
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_indexBuffer[i + 1] = std::stoi(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
#ifdef USING_OPENGL
		m_meshes.back().m_indexBuffer[i] = std::stoi(numString);
#else
		m_meshes.back().m_indexBuffer[i+2] = std::stoi(numString);
#endif // USING_OPENGL
		numString.clear();
		++m_pointer;
	}
	indexPos++;
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
	for (int normalPos = 0; normalPos < nNormals; normalPos++)
	{
		m_pointer += 2;
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_vbo[normalPos].nx = std::stof(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_vbo[normalPos].ny = std::stof(numString);
		numString.clear();

		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_vbo[normalPos].nz = std::stof(numString);
		m_meshes.back().m_vbo[normalPos].nw = 1;
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
	for (int textCordsPos = 0; textCordsPos < nTextCords; textCordsPos++)//nTextCords
	{
		++m_pointer;
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_vbo[textCordsPos].s = std::stof(numString);
		numString.clear();
		while (!(*m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_vbo[textCordsPos].t = std::stof(numString);
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
		while (!(*m_pointer == ',' || *m_pointer == ';'))
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
	std::string pathString;
	for (int i = 0; i < m_meshes.back().m_subsets.size(); i++)
	{
		//-------------------------------------------------------------//
		//Cargar todas las propiedades del Material
		pathString.clear();
		char* temp;
		//NoLighting"; 0;
		if ((temp = strstr(m_pointer, "NoLighting")))
		{
			m_pointer = temp;
			m_pointer++;
			while (!(*m_pointer++ == '\n'));
			m_pointer++;
			while (!(*m_pointer == ';'))
			{
				pathString.push_back(*m_pointer);
				++m_pointer;
			}
			m_pointer +=2;
			m_meshes.back().m_subsets[i].m_effects.m_noLighting = std::stoi(pathString);
		}
		pathString.clear();

		//diffuseMap
		if ((temp = strstr(m_pointer, "diffuseMap")))
		{
			m_pointer = temp;
			m_pointer++;
			while (!(*m_pointer++ == '"'));
			while (!(*m_pointer++ == '"'));
			//m_pointer++;
			while (!(*m_pointer == '"'))
			{
				pathString.push_back(*m_pointer);
				++m_pointer;
			}
			m_pointer += 2;
			size_t offset = pathString.find_first_of(' ');
			if (offset < pathString.size())
				pathString = pathString.substr(offset + 1);
			else
			{
				offset = pathString.find_first_of('\\');
				if (offset < pathString.size())
					pathString = pathString.substr(offset + 2);
			}
			m_meshes.back().m_subsets[i].m_effects.m_difusePath = pathString;
		}

		pathString.clear();

		//// speclevel"; 1; 1.0000;
		//if ((temp = strstr(m_pointer, "speclevel")))
		//{
		//	m_pointer = temp;
		//	m_pointer++;
		//	while (!(*m_pointer++ == '\n'));
		//	while (!(*m_pointer++ == '\n'));
		//	m_pointer++;
		//	while (!(*m_pointer == ';'))
		//	{
		//		pathString.push_back(*m_pointer);
		//		++m_pointer;
		//	}
		//	m_pointer += 2;
		//	m_meshes.back().m_subsets[i].m_effects.m_specLevel = std::stof(pathString);
		//}
		//pathString.clear();
		////specularMap
		if ((temp = strstr(m_pointer, "specularMap")))
		{
			if (temp - m_pointer < 1000)
			{
				m_pointer = temp;
				m_pointer++;
				while (!(*m_pointer++ == '"'));
				while (!(*m_pointer++ == '"'));
				m_pointer++;
				while (!(*m_pointer == '"'))
				{
					pathString.push_back(*m_pointer);
					++m_pointer;
				}
				m_pointer += 2;
				size_t offset = pathString.find_first_of(' ');
				if (offset < pathString.size())
					pathString = pathString.substr(offset + 1);
				else
				{
					offset = pathString.find_first_of('\\');
					if (offset < pathString.size())
						pathString = pathString.substr(offset + 2);
				}
				m_meshes.back().m_subsets[i].m_effects.m_specularMap = pathString;
			}
		}
		pathString.clear();
		//glossMap
		if ((temp = strstr(m_pointer, "glossMap")))
		{
			if (temp - m_pointer < 1000)
			{
				m_pointer = temp;
				m_pointer++;
				while (!(*m_pointer++ == '"'));
				while (!(*m_pointer++ == '"'));
				m_pointer++;
				while (!(*m_pointer == '"'))
				{
					pathString.push_back(*m_pointer);
					++m_pointer;
				}
				m_pointer += 2;
				size_t offset = pathString.find_first_of(' ');
				if (offset < pathString.size())
					pathString = pathString.substr(offset + 1);
				else
				{
					offset = pathString.find_first_of('\\');
					if (offset < pathString.size())
						pathString = pathString.substr(offset + 2);
				}
				m_meshes.back().m_subsets[i].m_effects.m_glossMap = pathString;
			}
		}
		pathString.clear();
		//glossiness"; 1; 2.0000;
		if ((temp = strstr(m_pointer, "glossiness")))
		{
			if (temp - m_pointer < 1000)
			{
				m_pointer = temp;
				m_pointer++;
				while (!(*m_pointer++ == '\n'));
				while (!(*m_pointer++ == '\n'));
				m_pointer++;
				while (!(*m_pointer == ';'))
				{
					pathString.push_back(*m_pointer);
					++m_pointer;
				}
				m_pointer += 2;
				m_meshes.back().m_subsets[i].m_effects.m_glossines = std::stof(pathString);
			}
		}
		pathString.clear();
		//normalMap
		if ((temp = strstr(m_pointer, "normalMap\"")))
		{
			if (temp - m_pointer < 1000)
			{
				m_pointer = temp;
				m_pointer++;
				while (!(*m_pointer++ == '"'));
				while (!(*m_pointer++ == '"'));
				m_pointer++;
				while (!(*m_pointer == '"'))
				{
					pathString.push_back(*m_pointer);
					++m_pointer;
				}
				m_pointer += 2;
				size_t offset = pathString.find_first_of(' ');
				if (offset < pathString.size())
					pathString = pathString.substr(offset + 1);
				else
				{
					offset = pathString.find_first_of('\\');
					if (offset < pathString.size())
						pathString = pathString.substr(offset + 2);
				}
				m_meshes.back().m_subsets[i].m_effects.m_normalMap = pathString;
			}
		}

		pathString.clear();
		// bFlipGreenChannel"; 1;
		if (temp = strstr(m_pointer, "bFlipGreenChannel"))
		if (temp - m_pointer < 1000)
		{
			m_pointer = temp;
			m_pointer++;
			while (!(*m_pointer++ == '\n'));
			m_pointer++;
			while (!(*m_pointer == ';'))
			{
				pathString.push_back(*m_pointer);
				++m_pointer;
			}
			m_pointer += 2;
			m_meshes.back().m_subsets[i].m_effects.m_blFlipGreenChanel = std::stoi(pathString);
		}
		pathString.clear();
		if (m_meshes.back().m_subsets[i].m_effects.m_difusePath == "")
			if (temp = strstr(m_pointer, "TextureFilename Diffuse")) {
				while (!(*m_pointer++ == '"'));
				while (!(*m_pointer == '"'))
				{
					pathString.push_back(*m_pointer);
					++m_pointer;
				}
				m_pointer += 2;
				size_t offset = pathString.find_first_of(' ');
				if (offset < pathString.size())
					pathString = pathString.substr(offset + 1);
				else
				{
					offset = pathString.find_first_of('\\');
					if (offset < pathString.size())
						pathString = pathString.substr(offset + 2);
				}
				m_meshes.back().m_subsets[i].m_effects.m_difusePath = pathString;
			}
		//-------------------------------------------------------------//
	}
}
void MeshParser::getDeclData()
{
	++m_pointer;
	std::string numString="";

	while (!(*m_pointer++ == '\n'));
	while (!(*m_pointer++ == '\n'));
	while (!(*m_pointer++ == '\n'));
	while (!(*m_pointer++ == '\n'));


	while (!(*m_pointer == ';'))
	{
		numString.push_back(*m_pointer);
		++m_pointer;
	}
	++m_pointer;
	int numElements = std::stoi(numString)/6 ;
	numString.clear();
	//m_meshes.back().m_tangents.resize(numElements);
	//m_meshes.back().m_binormals.resize(numElements);
	for (int declDataPos = 0; declDataPos < numElements; declDataPos++)
	{
		++m_pointer;
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		unsigned long long temp = std::stoul(numString);
		float *fval = (float*)&temp;
		m_meshes.back().m_vbo[declDataPos].bx = *fval;
		//m_meshes.back().m_tangents[i].x = std::stof(numString);
		numString.clear();
		++m_pointer;
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		temp = std::stoul(numString);
		fval = (float*)&temp;
		m_meshes.back().m_vbo[declDataPos].by = *fval;
		//m_meshes.back().m_tangents[i].y = std::stof(numString);
		numString.clear();
		++m_pointer;
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		temp = std::stoul(numString);
		fval = (float*)&temp;
		m_meshes.back().m_vbo[declDataPos].bz = *fval;
	//	m_meshes.back().m_tangents[i].z = std::stof(numString);
		numString.clear();
		++m_pointer;
		m_meshes.back().m_vbo[declDataPos].bw = 0;

		//++m_pointer;
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		temp = std::stoul(numString);
		fval = (float*)&temp;
		m_meshes.back().m_vbo[declDataPos].tx = *fval;
		//m_meshes.back().m_binormals[i].x = std::stof(numString);
		numString.clear();
		++m_pointer;
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		temp = std::stoul(numString);
		fval = (float*)&temp;
		m_meshes.back().m_vbo[declDataPos].ty = *fval;
		//m_meshes.back().m_binormals[i].y = std::stof(numString);
		numString.clear();
		++m_pointer;
		while (!(*m_pointer == ','))
		{
			if (*m_pointer == ';')
				break;
			numString.push_back(*m_pointer);
			++m_pointer;

		}
		++m_pointer;
		temp = std::stoul(numString);
		fval = (float*)&temp;
		m_meshes.back().m_vbo[declDataPos].tz = *fval;
		//m_meshes.back().m_binormals[i].z = std::stof(numString);
		numString.clear();
		m_meshes.back().m_vbo[declDataPos].tw = 0;
		++m_pointer;
	}

	/*for (int i = 0; i < numElements; i++)
	{*/
		/*++m_pointer;
		while (!(*m_pointer == ','))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_binormals[i].x = std::stof(numString);
		numString.clear();
		++m_pointer;
		while (!(*m_pointer == ',') )
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		m_meshes.back().m_binormals[i].y = std::stof(numString);
		numString.clear();
		++m_pointer;
		while (!(*m_pointer == ','))
		{
			if (*m_pointer == ';')
				break;
			numString.push_back(*m_pointer);
			++m_pointer;

		}
		++m_pointer;
		m_meshes.back().m_binormals[i].z = std::stof(numString);
		numString.clear();
		

		++m_pointer;*/
	//}
}
void MeshParser::getBones()
{
	static bool d = false;
	static int indx = -1;
	static int minIndx = 0;
	static std::vector<int> dads;
	m_pointer = strstr(m_pointer, "{");
	openBlocks++;
	bones.push_back(xBone());

	//
	if (!d) {
		actualDad = actualBone-1;
		indx++;
		dads.push_back(actualDad);
	}
	else {
		actualDad = dads[indx];
		for (size_t i = dads.size(); i > indx+1; i--)
		{
			dads.pop_back();
		}
	}

	d = false;

	bones.back().dad = actualDad;
	if (actualDad != -1)
		bones[actualDad].child.push_back(actualBone);



	bones.back().name = getName();
	bones.back().name = std::string(bones.back().name.rbegin(), bones.back().name.rend());
	bones.back().bone = getFrameTransformMatrix();
	ignoreObjectMatrixComment();
	if (IsNextACloseBlock())
		indx++;
	while (IsNextACloseBlock())
	{
		m_pointer = strstr(m_pointer, "}");
		if (m_pointer == nullptr)
			break;
		m_pointer++;
		actualDad--;
		indx--;
		openBlocks--;
		d = true;
	}
	if (openBlocks != 0) {
		actualBone++;
		getBones();
	}

}
void MeshParser::InsertBonesSiblingsOnEachBone()
{
	for (size_t bone1 = 1; bone1 < bones.size(); bone1++) {
		for (size_t bone2 = 1; bone2 < bones.size(); bone2++) {
			if (bones[bone1].dad == bones[bone2].dad)
			{
				bones[bone1].brothers.push_back(bone2);
			}
		}
	}
}
Matrix4D MeshParser::getFrameTransformMatrix()
{
	m_pointer++;
	m_pointer = strstr(m_pointer, "{");
	m_pointer+=2;
	Matrix4D TMatrix;
	int i = 0;
	while (*m_pointer != ';')
	{
		std::string numString;
		while (!(*m_pointer == ',' || *m_pointer == ';'))
		{
			numString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		float matrixCell = std::stof(numString);
		TMatrix.v[i++] = matrixCell;
	}
	m_pointer = strstr(m_pointer, "}");
	m_pointer++;
	return TMatrix;
}
void MeshParser::ignoreObjectMatrixComment()
{
	m_pointer  = strstr(m_pointer, "}");
	m_pointer++;
}

bool MeshParser::IsNextACloseBlock()
{
	char* toCloseBlock;
	char* toNewBone;

	toCloseBlock = strstr(m_pointer, "}");
	toNewBone = strstr(m_pointer, "{");
	if (toCloseBlock - m_pointer < toNewBone - m_pointer || toNewBone == nullptr)
		return true;
	return false;
}

void MeshParser::LoadWeights()
{
	m_pointer++;
	m_pointer = strstr(m_pointer, "\"");
	m_pointer++;
	std::string actualBone = LoadBoneName();
	static int actualBoneIndex = 0;
	for (int i =0 ; i < bones.size(); i++)
	{
		if (bones[i].name == actualBone)
		{
			actualBoneIndex = i;
			break;
		}
	}
	m_pointer = strstr(m_pointer, ";");
	m_pointer += 2;
	int numWeights = LoadNumWeights();
	m_pointer += 2;
	std::vector<int> wIndex = LoadWeightsIndex(numWeights, actualBoneIndex);
	m_pointer ++;
	LoadIndexWeights(numWeights, wIndex);
	for (int i = 0; i < m_meshes.back().m_vbo.size(); i++) {
		for (int j = 0; j < m_meshes.back().m_skinWeights[i].boneIndex.size(); j++)
		{
			m_meshes.back().m_vbo[i].wIndex[j] = m_meshes.back().m_skinWeights[i].boneIndex[j];
			m_meshes.back().m_vbo[i].wWeight[j] = m_meshes.back().m_skinWeights[i].weight[j];
		}
	}
	m_pointer++;
	//m_meshes.back().m_skinWeightsOffset.resize(bones.size());
	m_meshes.back().m_skinWeightsOffset[actualBoneIndex] =  LoadSpaceTransformMatrix();
	//actualBoneIndex++;
}

std::string MeshParser::LoadBoneName()
{
	std::string temp;
	while (*m_pointer != '\"')
	{
		temp.push_back(*m_pointer);
		++m_pointer;
	}
	return temp;
}

int MeshParser::LoadNumWeights()
{
	std::string temp;
	while (*m_pointer != ';')
	{
		temp.push_back(*m_pointer);
		++m_pointer;
	}
	return std::stoi(temp);
}

std::vector<int> MeshParser::LoadWeightsIndex(int numWeights, int actualBoneIndex)
{
	std::string indexString;
	std::vector<int> wIndexs;
	for (size_t i = 0; i < numWeights; i++)
	{
		indexString.clear();
		while (*m_pointer != ',' && *m_pointer != ';')
		{
			indexString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		int index = std::stoi(indexString);
		//index -= indexPos;
		m_meshes.back().m_skinWeights.resize(m_meshes.back().m_vbo.size());
		m_meshes.back().m_skinWeights[index].boneIndex.push_back(actualBoneIndex);
		wIndexs.push_back(index);
	}
	return wIndexs;
}

void MeshParser::LoadIndexWeights(int numWeights, std::vector<int>& index)
{
	std::string weightString;
	for (size_t i = 0; i < numWeights; i++)
	{
		weightString.clear();
		while (*m_pointer != ',' && *m_pointer != ';')
		{
			weightString.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		float w = std::stof(weightString);
		m_meshes.back().m_skinWeights[index[i]].weight.push_back(w);
	}
}

Matrix4D MeshParser::LoadSpaceTransformMatrix()
{
	Matrix4D spaceTransform;
	int i = 0;
	while (*m_pointer != ';')
	{
		std::string cellString;
		while (*m_pointer != ',' && *m_pointer != ';')
		{
			cellString.push_back(*m_pointer);
			++m_pointer;
		}
		m_pointer++;
		float cell = std::stof(cellString);
		spaceTransform.v[i++] = cell;
	}
	return spaceTransform;
}

void MeshParser::ReadAnimationTicksPerSecond()
{
	m_pointer++;
	std::string temp;
	while (*m_pointer != ';')
	{
		temp.push_back(*m_pointer);
		++m_pointer;
	}
	m_pointer++;
	animationSets.back().ticksPS = 	std::stoi(temp);
}

void MeshParser::ReadAnimationSet()
{
	static int actualBoneIndex = 0;
	while (actualBoneIndex < bones.size())
	{
		m_pointer++;
		m_pointer = strstr(m_pointer, "{");
		m_pointer = strstr(m_pointer, "{");
		m_pointer++;
		std::string boneName;
		while (*m_pointer != '}')
		{
			boneName.push_back(*m_pointer);
			++m_pointer;
		}
		++m_pointer;
		animationSets.back().animationsVec.push_back(xAnimation());
		animationSets.back().animationsVec.back().name = boneName;
		animationSets.back().animationsVec.back().boneIndex = actualBoneIndex;
		//Read rot
		ReadAnimationRotations();
		//Read scale
		m_pointer = strstr(m_pointer, "{");
		m_pointer++;
		ReadAnimationScales();
		//Read pos
		m_pointer = strstr(m_pointer, "{");
		m_pointer++;
		ReadAnimationPositions();
		animationSets.back().animationsVec.back().maxIndex = animationSets.back().animationsVec.back().rotVec.size();
		if (animationSets.back().animationsVec.back().posVec.size() > animationSets.back().animationsVec.back().maxIndex)
			animationSets.back().animationsVec.back().maxIndex = animationSets.back().animationsVec.back().posVec.size();
		if (animationSets.back().animationsVec.back().scaleVec.size() > animationSets.back().animationsVec.back().maxIndex)
			animationSets.back().animationsVec.back().maxIndex = animationSets.back().animationsVec.back().scaleVec.size();
		actualBoneIndex++;
	}

}

void MeshParser::ReadAnimationRotations()
{
	m_pointer = strstr(m_pointer, ";");
	m_pointer++;
	//Read num keyframes
	std::string temp;
	while (*m_pointer != ';')
	{
		temp.push_back(*m_pointer);
		++m_pointer;
	}
	m_pointer++;
	int n = std::stoi(temp);

	for (size_t i = 0; i < n; i++)
	{
		//ReadTime
		int time = ReadAnimTime();
		m_pointer = strstr(m_pointer, ";");
		m_pointer++;
		//Read 4 floats
		float f1 = readAnimFloat();
		float f2 = readAnimFloat();
		float f3 = readAnimFloat();
		float f4 = readAnimFloat();
		m_pointer+=3;
		xAnimationKeyRot rot;
		rot.QRot = Vector4D(f1, f2, f3, f4);
		rot.ticks = time;
		int index = time / 160;
		if (animationSets.back().animationsVec.back().rotVec.size() < index + 1) {
			int tmp = animationSets.back().animationsVec.back().rotVec.size()-1;
			animationSets.back().animationsVec.back().rotVec.resize(index + 1);
			for (size_t j = tmp; j < index; j++)
			{
				animationSets.back().animationsVec.back().rotVec[j].QRot = animationSets.back().animationsVec.back().rotVec[tmp].QRot;
				//animationSets.back().animationsVec.back().rotVec[j].ticks = animationSets.back().animationsVec.back().rotVec[tmp].ticks + 160;
			}
		}

		animationSets.back().animationsVec.back().rotVec[index] = rot;
		//animationSets.back().animationsVec.back().rotVec.push_back(rot);
	}
}



void MeshParser::ReadAnimationPositions()
{
	m_pointer = strstr(m_pointer, ";");
	m_pointer++;
	//Read num keyframes
	std::string temp;
	while (*m_pointer != ';')
	{
		temp.push_back(*m_pointer);
		++m_pointer;
	}
	m_pointer++;
	int n = std::stoi(temp);

	for (size_t i = 0; i < n; i++)
	{
		//ReadTime
		int time = ReadAnimTime();
		m_pointer = strstr(m_pointer, ";");
		m_pointer++;
		//Read 4 floats
		float f1 = readAnimFloat();
		float f2 = readAnimFloat();
		float f3 = readAnimFloat();
		m_pointer += 3;
		xAnimationKeyPos sc;
		sc.pos = Vector3D(f1, f2, f3);
		sc.ticks = time;
		int index = time / 160;
		if (animationSets.back().animationsVec.back().posVec.size() < index + 1) {
			int tmp = animationSets.back().animationsVec.back().posVec.size() - 1;
			animationSets.back().animationsVec.back().posVec.resize(index + 1);
			for (size_t j = tmp; j < index; j++)
			{
				animationSets.back().animationsVec.back().posVec[j].pos = animationSets.back().animationsVec.back().posVec[tmp].pos;
				//animationSets.back().animationsVec.back().rotVec[j].ticks = animationSets.back().animationsVec.back().rotVec[tmp].ticks + 160;
			}
		}
		animationSets.back().animationsVec.back().posVec[index] = sc;
		//animationSets.back().animationsVec.back().posVec.push_back(sc);
	}
}

void MeshParser::ReadAnimationScales()
{
	m_pointer = strstr(m_pointer, ";");
	m_pointer++;
	//Read num keyframes
	std::string temp;
	while (*m_pointer != ';')
	{
		temp.push_back(*m_pointer);
		++m_pointer;
	}
	m_pointer++;
	int n = std::stoi(temp);

	for (size_t i = 0; i < n; i++)
	{
		//ReadTime
		int time = ReadAnimTime();
		m_pointer = strstr(m_pointer, ";");
		m_pointer++;
		//Read 4 floats
		float f1 = readAnimFloat();
		float f2 = readAnimFloat();
		float f3 = readAnimFloat();
		m_pointer += 3;
		xAnimationKeyScale sc;
		sc.scale = Vector3D(f1, f2, f3);
		sc.ticks = time;
		int index = time / 160;
		if (animationSets.back().animationsVec.back().scaleVec.size() < index + 1) {
			int tmp = animationSets.back().animationsVec.back().scaleVec.size() - 1;
			animationSets.back().animationsVec.back().scaleVec.resize(index + 1);
			for (size_t j = tmp; j < index; j++)
			{
				animationSets.back().animationsVec.back().scaleVec[j].scale = animationSets.back().animationsVec.back().scaleVec[tmp].scale;
				//animationSets.back().animationsVec.back().rotVec[j].ticks = animationSets.back().animationsVec.back().rotVec[tmp].ticks + 160;
			}
		}
		animationSets.back().animationsVec.back().scaleVec[index] = sc;
		//animationSets.back().animationsVec.back().scaleVec.push_back(sc);
	}
}

int MeshParser::ReadAnimTime()
{
	std::string timestr;
	while (*m_pointer != ';')
	{
		timestr.push_back(*m_pointer);
		++m_pointer;
	}
	m_pointer++;
	return std::stoi(timestr);
}

float MeshParser::readAnimFloat()
{
	std::string fstr;
	while (*m_pointer != ',' && *m_pointer != ';')
	{
		fstr.push_back(*m_pointer);
		++m_pointer;
	}
	m_pointer++;
	return std::stof(fstr);
}


void MeshParser::Deallocate()
{
	for(auto &it : m_meshes)
		it.m_vbo.clear();
	//m_meshes.clear();
}

