
#include "Vector3D.h"

/*
	create a vector from 3 float values as parameters
*/
Vector3D::Vector3D(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

/*
	create a vector from another vector
*/
Vector3D::Vector3D(const Vector3D& _vector)
{
	x = _vector.x;
	y = _vector.y;
	z = _vector.z;
}


//operator +;
Vector3D Vector3D::operator+(Vector3D _vector)
{
	return Vector3D(x + _vector.x, y + _vector.y, z);
}


//operator -;
Vector3D Vector3D::operator-(Vector3D& _vector)
{
	return Vector3D(x - _vector.x, y - _vector.y, z - _vector.z);
}


// multiplication by scalar
Vector3D Vector3D::operator*(float _a)
{
	return Vector3D(_a*x, _a*y, _a*z);
}


// division by scalar, if a division by 0 then it returns the same vector
Vector3D Vector3D::operator/(float _a)
{
	return _a!=0? Vector3D(x/_a, y/_a, z/_a) : Vector3D(x,y,z);
}


// dot product between two vectors
float Vector3D::dotProduct(const Vector3D _vector)
{
	return (_vector.x * x) + (_vector.y * y) + (_vector.z * z);
}

// cross product between two vectors;
Vector3D Vector3D::crossProduct(const Vector3D _vector)
{
	return Vector3D( y*_vector.z - z*_vector.y, z*_vector.x - x*_vector.z, x*_vector.y - y*_vector.x);
}

// calculate the lenght of the vector
float Vector3D::vectorLength()
{
	return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

// retorna o comprimento do vetor
float Vector3D::getVectorLength()
{
	return vectorLength();
}

//normalize the vector: make the lenght equals to 1
Vector3D Vector3D::normalize()
{
	float  vecLength = this->vectorLength();
	return Vector3D( x / vecLength, y / vecLength, z / vecLength);
}
