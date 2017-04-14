#include "Sprite.h"

void Sprite::TranslateAbsolute(float x, float y, float z) {
	Position = Translation(x, y, z);
}
void Sprite::ScaleAbsolute(float sc) {
	Scale = Scaling(sc, sc, sc);
}
void Sprite::Update() {
	transform = Scale*Position;
}
void Sprite::SetTexture(char * str)
{
	m_str = str;
}

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}
