#include "TextMesh.h"


void TextMesh::TranslateAbsolute(float x, float y, float z) {
	Position = Translation(x, y, z);
}
void TextMesh::Update() {
	transform = Scale*Position;
}
TextMesh::TextMesh()
{
}


TextMesh::~TextMesh()
{
}
