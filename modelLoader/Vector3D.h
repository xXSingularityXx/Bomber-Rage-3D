#include <cmath>

#ifndef VECTORS_H
#define VECTORS_H

/*
	Store the coordinates of a vector in 3D space.
*/

class Vector3D
{
	public:
	float x, y, z;

	Vector3D(){}
	Vector3D(const Vector3D& _vector);
	Vector3D(float _x, float _y, float _z);

	~Vector3D(){}

	Vector3D operator+(Vector3D _vector);
	Vector3D operator-(Vector3D& _vector);
	Vector3D operator*(float _a);
	Vector3D operator/(float _a);

	float dotProduct(const Vector3D _vector);
	Vector3D crossProduct(const Vector3D _vector);
	Vector3D normalize();
	float getVectorLength();

	private:
	float vectorLength();
};

#endif
