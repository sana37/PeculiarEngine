#include "Calculater.h"
#include "Vector.h"
#include <math.h>


bool Calculater::solveCubicEquation(float* a , float* b , float* c , float* k , float* ans)
{
	float det = a[0]*b[1]*c[2] + a[1]*b[2]*c[0] + a[2]*b[0]*c[1] - a[2]*b[1]*c[0] - a[1]*b[0]*c[2] - a[0]*b[2]*c[1];

	if(det == 0){
//		std::cerr << "ERROR : det = 0\n";
		return(false);
	}

	ans[0] = ((b[1]*c[2] - b[2]*c[1])*k[0] + (b[2]*c[0] - b[0]*c[2])*k[1] + (b[0]*c[1] - b[1]*c[0])*k[2]) / det;
	ans[1] = ((c[1]*a[2] - c[2]*a[1])*k[0] + (c[2]*a[0] - c[0]*a[2])*k[1] + (c[0]*a[1] - c[1]*a[0])*k[2]) / det;
	ans[2] = ((a[1]*b[2] - a[2]*b[1])*k[0] + (a[2]*b[0] - a[0]*b[2])*k[1] + (a[0]*b[1] - a[1]*b[0])*k[2]) / det;

	return(true);
}

void Calculater::rotate(Vector* vertex , Vector* shaft , float deg)
{
	float temp[3];
	float sx = shaft->getX();
	float sy = shaft->getY();
	float sz = shaft->getZ();
	float vx = vertex->getX();
	float vy = vertex->getY();
	float vz = vertex->getZ();
//	float r = shaft->getMagnitude();//sqrt(sx*sx + sy*sy + sz*sz);

	if(deg == 0)
		return;

	deg *= PI / 180;
/*
	sx /= r;
	sy /= r;
	sz /= r;
*/
	temp[0] = matrix1(sx , deg)*vx + matrix2(sx , sy , -sz , deg)*vy + matrix2(sx , sz , sy , deg)*vz;
	temp[1] = matrix2(sx , sy , sz , deg)*vx + matrix1(sy , deg)*vy + matrix2(sy , sz , -sx , deg)*vz;
	temp[2] = matrix2(sx , sz , -sy , deg)*vx + matrix2(sy , sz , sx , deg)*vy + matrix1(sz , deg)*vz;

	vertex->setVector(temp);
}

float Calculater::matrix1(float m , float deg)
{
	return(m*m + (1 - m*m)*cos(deg));
}

float Calculater::matrix2(float l , float m , float n , float deg)
{
	return(l*m*(1 - cos(deg)) + n*sin(deg));
}
