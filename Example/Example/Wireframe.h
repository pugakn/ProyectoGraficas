#pragma once
#include "MeshParser.h"
struct WireframeVertex {
	float x, y, z, w;
};

class Wireframe
{
public:
	void CreateWireframe( std::vector<unsigned short> indexBuffer );
	std::vector<unsigned short> m_indexBuffer;
	Wireframe();
	~Wireframe();
};

