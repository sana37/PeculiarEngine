#ifndef UNIVERSALFORCE_H
#define UNIVERSALFORCE_H

#include "Force.h"

class Object;

class UniversalForce : public Force
{
public:
	explicit UniversalForce(Object*);
	UniversalForce(const UniversalForce&);
	virtual ~UniversalForce(void);

	virtual void exec(void);
	virtual bool isDone(void);
	virtual Vector getForcePoint(void);
	void changeObject(Object*);

private:
	Object* object;
};

#endif
