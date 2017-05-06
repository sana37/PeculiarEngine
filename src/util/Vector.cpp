#include "Vector.h"
#include <math.h>
//#include <iostream>//atodekesu

Vector::Vector(void)
{
	X = 0;
	Y = 0;
	Z = 0;
}

Vector::Vector(float* factor)
{
	X = factor[0];
	Y = factor[1];
	Z = factor[2];
}

Vector::Vector(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector::Vector(Vector* vector)
{
	X = vector->getX();
	Y = vector->getY();
	Z = vector->getZ();
}

Vector::Vector(const Vector& vector)
{
	X = vector.getX();
	Y = vector.getY();
	Z = vector.getZ();
}

Vector::~Vector(void)
{
}

float Vector::getX(void) const
{
	return X;
}

float Vector::getY(void) const
{
	return Y;
}

float Vector::getZ(void) const
{
	return Z;
}

float Vector::getMagnitude(void) const
{
	return sqrt(X*X + Y*Y + Z*Z);
}

void Vector::getVector(float* factor) const
{
	factor[0] = X;
	factor[1] = Y;
	factor[2] = Z;
}

void Vector::setX(float x)
{
	X = x;
}

void Vector::setY(float y)
{
	Y = y;
}

void Vector::setZ(float z)
{
	Z = z;
}

void Vector::setVector(float* factor)
{
	X = factor[0];
	Y = factor[1];
	Z = factor[2];
}

void Vector::setVector(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

void Vector::setVector(Vector* vector)
{
	X = vector->getX();
	Y = vector->getY();
	Z = vector->getZ();
}

Vector Vector::operator+(const Vector& addedVector) const
{
	return Vector(X + addedVector.getX(), Y + addedVector.getY(), Z + addedVector.getZ());
}

Vector Vector::operator-(const Vector& subtractedVector) const
{
	return Vector(X - subtractedVector.getX(), Y - subtractedVector.getY(), Z - subtractedVector.getZ());
}

Vector Vector::operator*(float k) const
{
	return Vector(X * k, Y * k, Z * k);
}

Vector Vector::operator/(float k) const
{
	return Vector(X / k, Y / k, Z / k);
}

float Vector::operator*(const Vector& vector) const
{
	return (X * vector.X) + (Y * vector.Y) + (Z * vector.Z);
}

Vector Vector::operator%(const Vector& vector) const
{
	float a[3], b[3];

	this->getVector(a);
	vector.getVector(b);

	return Vector(
		a[1]*b[2] - b[1]*a[2],
		a[2]*b[0] - b[2]*a[0],
		a[0]*b[1] - b[0]*a[1]
	);
}

const Vector& Vector::operator=(const Vector& vector)
{
	X = vector.getX();
	Y = vector.getY();
	Z = vector.getZ();
	return *this;
}

const Vector& Vector::operator+=(const Vector& vector)
{
	X += vector.getX();
	Y += vector.getY();
	Z += vector.getZ();
	return *this;
}

const Vector& Vector::operator-=(const Vector& vector)
{
	X -= vector.getX();
	Y -= vector.getY();
	Z -= vector.getZ();
	return *this;
}

const Vector& Vector::operator*=(float k)
{
	X *= k;
	Y *= k;
	Z *= k;
	return *this;
}

const Vector& Vector::operator/=(float k)
{
	X /= k;
	Y /= k;
	Z /= k;
	return *this;
}
