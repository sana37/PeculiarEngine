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

Vector::Vector(float x , float y , float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector::Vector(Vector* originalVector)
{
	X = originalVector->getX();
	Y = originalVector->getY();
	Z = originalVector->getZ();
}

Vector::Vector(const Vector& originalVector)
{
//	std::cerr << "copy constructer\n";
	X = originalVector.getX();
	Y = originalVector.getY();
	Z = originalVector.getZ();
}

Vector::~Vector(void)
{
}

float Vector::getX(void) const
{
	return(X);
}

float Vector::getY(void) const
{
	return(Y);
}

float Vector::getZ(void) const
{
	return(Z);
}

float Vector::getMagnitude(void) const
{
	return(sqrt(X*X + Y*Y + Z*Z));
}

void Vector::getVector(float* factor)
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

void Vector::setVector(float x , float y , float z)
{
	X = x;
	Y = y;
	Z = z;
}

void Vector::setVector(Vector* originalVector)
{
	X = originalVector->getX();
	Y = originalVector->getY();
	Z = originalVector->getZ();
}

void Vector::addVector(float* factor)
{
	X += factor[0];
	Y += factor[1];
	Z += factor[2];
}

void Vector::addVector(float x , float y , float z)
{
	X += x;
	Y += y;
	Z += z;
}

void Vector::addVector(Vector* addedVector)
{
	X += addedVector->getX();
	Y += addedVector->getY();
	Z += addedVector->getZ();
}

void Vector::subtractVector(float* factor)
{
	X -= factor[0];
	Y -= factor[1];
	Z -= factor[2];
}

void Vector::subtractVector(float x , float y , float z)
{
	X -= x;
	Y -= y;
	Z -= z;
}

void Vector::subtractVector(Vector* subtractedVector)
{
	X -= subtractedVector->getX();
	Y -= subtractedVector->getY();
	Z -= subtractedVector->getZ();
}

void Vector::multiply(float k)
{
	X *= k;
	Y *= k;
	Z *= k;
}

void Vector::addVector(const Vector* vector1 , const Vector* vector2 , Vector* vectorAns)
{
	vectorAns->setX(vector1->getX() + vector2->getX());
	vectorAns->setY(vector1->getY() + vector2->getY());
	vectorAns->setZ(vector1->getZ() + vector2->getZ());
}

void Vector::subtractVector(const Vector* vector1 , const Vector* vector2 , Vector* vectorAns)
{
	vectorAns->setX(vector1->getX() - vector2->getX());
	vectorAns->setY(vector1->getY() - vector2->getY());
	vectorAns->setZ(vector1->getZ() - vector2->getZ());
}


Vector Vector::operator+(const Vector& addedVector) const
{
	return(Vector(X + addedVector.getX() , Y + addedVector.getY() , Z + addedVector.getZ()));
}

Vector Vector::operator-(const Vector& subtractedVector) const
{
	return(Vector(X - subtractedVector.getX() , Y - subtractedVector.getY() , Z - subtractedVector.getZ()));
}

Vector Vector::operator*(float k) const
{
	return(Vector(X * k , Y * k , Z * k));
}

const Vector& Vector::operator=(const Vector& originalVector)
{
	X = originalVector.getX();
	Y = originalVector.getY();
	Z = originalVector.getZ();
	return(*this);
}

const Vector& Vector::operator+=(const Vector& vector)
{
	X += vector.getX();
	Y += vector.getY();
	Z += vector.getZ();
	return(*this);
}

const Vector& Vector::operator-=(const Vector& vector)
{
	X -= vector.getX();
	Y -= vector.getY();
	Z -= vector.getZ();
	return(*this);
}

const Vector& Vector::operator*=(float k)
{
	X *= k;
	Y *= k;
	Z *= k;
	return(*this);
}
