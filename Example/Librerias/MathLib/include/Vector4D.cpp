#include "Vector4D.h"


Vector4D::Vector4D()
{
}
Vector4D::Vector4D(const Vector4D &V)
{
	x = V.x;
	y = V.y;
	z = V.z;
	w = V.w;
}
Vector4D::Vector4D(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}
Vector4D::~Vector4D()
{
}


ostream& operator << (ostream& out, const Vector4D& V) {
	out << "["<< V.x << "," << V.y << "," << V.z << "," << V.w << "]";
	return out;
}
istream& operator >> (istream& in, Vector4D& V) {
	in >> V.x >> V.y >> V.z >> V.w;
	return in;
}
Vector4D operator* (const Vector4D &A, const Vector4D &B) {
	return Vector4D(A.x*B.x, A.y*B.y, A.z*B.z, A.w*B.w);
}
Vector4D operator* (float s, const Vector4D&A) {
	return Vector4D(A.x*s,A.y*s,A.z*s,A.w*s);
}
Vector4D operator* (const Vector4D &A, float s) {
	return s*A;
}
Vector4D operator/ (const Vector4D &A, float s) {
	return A*(1 / s);
}
Vector4D operator+ (const Vector4D &A, const Vector4D &B) {
	return Vector4D(A.x + B.x, A.y + B.y,A.z + B.z,A.w + B.w);
}
Vector4D operator- (const Vector4D &A, const Vector4D &B) {
	return Vector4D(A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w);
}
float Dot(const Vector4D &A, const Vector4D &B) {
	return A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;
}
Vector4D Cross3(const Vector4D &A, const Vector4D &B) {
	return Vector4D(A.y*B.z - A.z*B.y,
					A.z*B.x - A.x*B.z,
					A.x*B.y - A.y*B.x,
					0);
}

float Magnity(const Vector4D &A) {
	return sqrt(Dot(A,A));
}
Vector4D Normalize(const Vector4D &A) {
	return A / Magnity(A);
}
