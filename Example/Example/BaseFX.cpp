#include "BaseFX.h"



BaseFX::BaseFX()
{
}


BaseFX::~BaseFX()
{
}

void BaseFX::Init()
{
	vertices[0] = { -1.0f,  1.0f,  0.9f ,1.0f,  0.0f, 0.0f };//Left Top
	vertices[1] = { -1.0f,  -1.0f, 0.9f ,1.0f,  0.0f, 1.0f };//Left Bot
	vertices[2] = { 1.0f,  -1.0f,  0.9f ,1.0f,  1.0f, 1.0f };//Right Bot
	vertices[3] = { 1.0f,  1.0f,   0.9f ,1.0f,  1.0f, 0.0f };//Right Top


	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;
#ifndef USING_D3D11
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(CVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#else
#endif

	InitAPI();
}
