#include "Wireframe.h"



void Wireframe::CreateWireframe( std::vector<unsigned short> indexBuffer)
{
	for (size_t i = 0; i < indexBuffer.size() ; i+=3) {
		m_indexBuffer.push_back(indexBuffer[i]);
		m_indexBuffer.push_back(indexBuffer[i + 1]);

		m_indexBuffer.push_back(indexBuffer[i]);
		m_indexBuffer.push_back(indexBuffer[i + 2]);

		m_indexBuffer.push_back(indexBuffer[i + 1]);
		m_indexBuffer.push_back(indexBuffer[i + 2]);
	}

}

Wireframe::Wireframe()
{
}


Wireframe::~Wireframe()
{
}
