#ifndef GRAVITY_H
#define GRAVITY_H

#define GRAVITY		0.004

#include "Force.h"

class Object;

class Gravity : public Force
{
public:
	explicit Gravity(Object*);
	Gravity(const Gravity&);
	virtual ~Gravity(void);

	void exec(void);

private:
	Object* obj;
};

#endif
