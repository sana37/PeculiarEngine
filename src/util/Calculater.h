#ifndef CALCULATER_H
#define CALCULATER_H

#define PI 3.141592


class Vector;

class Calculater
{
public:
	static bool solveCubicEquation(const Vector&, const Vector&, const Vector&, const Vector&, Vector*);
	static void rotateRad(Vector*, const Vector&, const Vector&, float);
	static float matrix1(float, float);
	static float matrix2(float, float, float, float);
//	static bool calculate1(const Vector&, const Vector&, Vector*);
};

#endif
