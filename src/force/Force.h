#ifndef FORCE_H
#define FORCE_H

#include "Vector.h"

class Force : public Vector
{
public:
	explicit Force(Vector);
	Force(const Force&);
	virtual ~Force(void);

	virtual void exec(void) = 0;
/*
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
*/
};

#endif
