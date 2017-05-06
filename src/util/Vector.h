#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:
	Vector(void);
	Vector(float*);
	Vector(float, float, float);
	Vector(Vector*);
	Vector(const Vector&);
	virtual ~Vector(void);

	float getX(void) const;
	float getY(void) const;
	float getZ(void) const;
	float getMagnitude(void) const;
	void getVector(float*) const;
	void setX(float);
	void setY(float);
	void setZ(float);
	void setVector(float*);
	void setVector(float, float, float);
	void setVector(Vector*);

	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;
	Vector operator*(float) const;
	Vector operator/(float) const;
	float operator*(const Vector&) const;
	Vector operator%(const Vector&) const;
	const Vector& operator=(const Vector&);
	const Vector& operator+=(const Vector&);
	const Vector& operator-=(const Vector&);
	const Vector& operator*=(float);
	const Vector& operator/=(float);

private:
	float X;
	float Y;
	float Z;
};

#endif
