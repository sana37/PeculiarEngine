#ifndef IMPULSE_H
#define IMPULSE_H

#define LEAST_DIST	0.05
#define K1			0.000001

#include "Force.h"

class Object;

//class which present force from obj2 to obj1.(force from obj1 to obj2 is presented as instance * -1)
//tukaisute
class Impulse : public Force
{
public:
//	Impulse(Vector, Vector, Object*, Object*);
	Impulse(const Vector&, const Vector&, Object*, Object*);
	Impulse(const Impulse&);
	virtual ~Impulse(void);

	void exec(void);
	void applyDecomposedForce(Object*);
	bool applySmallForce(void);
	float kakikukeko(const Vector&, Object*);
	float aiueo(const Vector&, Object*);
//	Vector getForcePoint(void);

private:
	Object* obj1;
	Object* obj2;
	Vector rv0;
	Vector forcePoint;
};

#endif
