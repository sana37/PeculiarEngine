#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:
	Vector(void);
	Vector(float*);
	Vector(float , float , float);
	Vector(Vector*);
	Vector(const Vector&);//copy constructer
	~Vector(void);

	float getX(void) const;
	float getY(void) const;
	float getZ(void) const;
	float getMagnitude(void) const;
	void getVector(float*);
	void setX(float);
	void setY(float);
	void setZ(float);
	void setVector(float*);
	void setVector(float , float , float);
	void setVector(Vector*);
	void addVector(float*);
	void addVector(float , float , float);
	void addVector(Vector*);
	void subtractVector(float*);
	void subtractVector(float , float , float);
	void subtractVector(Vector*);
	void multiply(float);

	static void addVector(const Vector* , const Vector* , Vector*);
	static void subtractVector(const Vector* , const Vector* , Vector*);

	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;
	Vector operator*(float) const;
	const Vector& operator=(const Vector&);
	const Vector& operator+=(const Vector&);
	const Vector& operator-=(const Vector&);
	const Vector& operator*=(float);

private:
	float X;
	float Y;
	float Z;
};

#endif
