#include "Calculater.h"
#include "Vector.h"
#include <iostream>
#include <math.h>


bool Calculater::solveCubicEquation(const Vector& A, const Vector& B, const Vector& C, const Vector& K, Vector* ans)
{
	float a[3], b[3], c[3], k[3];

	A.getVector(a);
	B.getVector(b);
	C.getVector(c);
	K.getVector(k);

	float det = a[0]*b[1]*c[2] + a[1]*b[2]*c[0] + a[2]*b[0]*c[1] - a[2]*b[1]*c[0] - a[1]*b[0]*c[2] - a[0]*b[2]*c[1];

	if (det == 0) {
//		std::cerr << "ERROR : det = 0\n";
		return false;
	}

	ans->setX(
		((b[1]*c[2] - b[2]*c[1])*k[0] + (b[2]*c[0] - b[0]*c[2])*k[1] + (b[0]*c[1] - b[1]*c[0])*k[2]) / det);
	ans->setY(
		((c[1]*a[2] - c[2]*a[1])*k[0] + (c[2]*a[0] - c[0]*a[2])*k[1] + (c[0]*a[1] - c[1]*a[0])*k[2]) / det);
	ans->setZ(
		((a[1]*b[2] - a[2]*b[1])*k[0] + (a[2]*b[0] - a[0]*b[2])*k[1] + (a[0]*b[1] - a[1]*b[0])*k[2]) / det);

	return true;
}

void Calculater::rotateRad(Vector* vertex_p, const Vector& offset, const Vector& shaft, float rad)
{
	if (rad == 0)
		return;

	*vertex_p -= offset;

	float temp[3];
	float sx = shaft.getX();
	float sy = shaft.getY();
	float sz = shaft.getZ();
	float vx = vertex_p->getX();
	float vy = vertex_p->getY();
	float vz = vertex_p->getZ();
//	float r = shaft.getMagnitude();//sqrt(sx*sx + sy*sy + sz*sz);

/*
	sx /= r;
	sy /= r;
	sz /= r;
*/
	temp[0] = matrix1(sx, rad)*vx + matrix2(sx, sy, -sz, rad)*vy + matrix2(sx, sz, sy, rad)*vz;
	temp[1] = matrix2(sx, sy, sz, rad)*vx + matrix1(sy, rad)*vy + matrix2(sy, sz, -sx, rad)*vz;
	temp[2] = matrix2(sx, sz, -sy, rad)*vx + matrix2(sy, sz, sx, rad)*vy + matrix1(sz, rad)*vz;

	vertex_p->setVector(temp);
	*vertex_p += offset;
}

float Calculater::matrix1(float m, float rad)
{
	return m*m + (1 - m*m)*cos(rad);
}

float Calculater::matrix2(float l, float m, float n, float rad)
{
	return l*m*(1 - cos(rad)) + n*sin(rad);
}
/*
bool Calculater::calculate1(const Vector& A, const Vector& B, Vector* ansVector)
{
	short s = 0;
	short t = 1;
	short u = 2;
	float a[3], b[3];

	A.getVector(a);
	B.getVector(b);

	for (short i = 0  ;  i < 6  ;  i++) {
		switch (i) {
			case 1 : {
				s = 0;
				t = 2;
				u = 1;
				break;
			}
			case 2 : {
				s = 1;
				t = 0;
				u = 2;
				break;
			}
			case 3 : {
				s = 1;
				t = 2;
				u = 0;
				break;
			}
			case 4 : {
				s = 2;
				t = 0;
				u = 1;
				break;
			}
			case 5 : {
				s = 2;
				t = 1;
				u = 0;
				break;
			}
		}
		if (a[s] == 0  ||  a[t] == 0) {
			if (i == 5) {
				return false;
			}
		} else if ((b[s] - a[s]/a[t]*b[t]) == 0  ||  (b[t] - a[t]/a[s]*b[s]) == 0) {
			if (i == 5) {
				return false;
			}
		} else {
//			std::cerr << "i:" << i << '\n';
			break;
		}
	}

	float temp1 = (a[u]/a[t]*b[t] - b[u]) / (b[s] - a[s]/a[t]*b[t]);
	float temp2 = (a[u]/a[s]*b[s] - b[u]) / (b[t] - a[t]/a[s]*b[s]);
	float ans[3];

	ans[u] = sqrt(1 / (temp1*temp1 + temp2*temp2 + 1));
	ans[s] = temp1 * ans[u];
	ans[t] = temp2 * ans[u];

	ansVector->setVector(ans);
	return true;
}
*/