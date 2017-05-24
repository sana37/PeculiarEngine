#ifndef IMPULSE_H
#define IMPULSE_H

#include "Force.h"

class Object;

//class which present force from obj2 to obj1.(force from obj1 to obj2 is presented as instance * -1)
//tukaisute
class Impulse : public Force
{
public:
	Impulse(const Vector&, const Vector&, Object*, Object*);
	Impulse(const Impulse&);
	virtual ~Impulse(void);

	void exec(void);
	bool isDone(void);

	void applyForceWithEnergy(void);
	void applyDecomposedForce(Object*, const Vector&);
	float kakikukeko(const Vector&, Object*);
	Vector getForcePoint(void);

private:
	Object* obj1;
	Object* obj2;
	Vector base;
	Vector forcePoint;
	bool done;
};

#endif
