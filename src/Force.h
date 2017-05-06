#ifndef FORCE_H
#define FORCE_H

#include "Vector.h"

class Object;

//class which present force from obj2 to obj1.(force from obj1 to obj2 is presented as instance * -1)
//tukaisute
class Force : public Vector
{
public:
	Force(Vector, Vector, Object*, Object*);
	Force(const Force&);
	virtual ~Force(void);

	void exec(void);
	void applyDecomposedForce(Object*);
	void setPermanent(bool);
	void enSet(void);
	void disSet(void);
	bool isPermanent(void);
	bool isSetted(void);
	Vector getForcePoint(void);

private:
	Object* obj1;
	Object* obj2;
	Vector forcePoint;
	bool permanent;
	bool setted;

};

#endif
