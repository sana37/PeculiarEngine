#ifndef UNIVERSALTORQUE_H
#define UNIVERSALTORQUE_H

#include "Force.h"

class Object;

class UniversalTorque : public Force
{
public:
	explicit UniversalTorque(Object*);
	UniversalTorque(const UniversalTorque&);
	virtual ~UniversalTorque(void);

	virtual void exec(void);
	virtual bool isDone(void);
	virtual Vector getForcePoint(void);
	void changeObject(Object*);

private:
	Object* object;
};

#endif
