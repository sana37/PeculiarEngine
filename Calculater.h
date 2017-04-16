#ifndef CALCULATER_H
#define CALCULATER_H

#define PI 3.141592


class Vector;

class Calculater
{
public:
	static bool solveCubicEquation(float* , float* , float* , float* , float*);
	static void rotate(Vector* , Vector* , float);
	static float matrix1(float , float);
	static float matrix2(float , float , float , float);
};

#endif
