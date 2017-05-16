#ifndef GRAVITY_H
#define GRAVITY_H

#define GRAVITY		0.004
//#define GRAVITY		0.098//9.8 * TIMEUNIT * 1000
//integrated define file is needed

#include "Force.h"

class Object;

class Gravity : public Force
{
public:
	explicit Gravity(Object*);
	Gravity(const Gravity&);
	virtual ~Gravity(void);

	void exec(void);
	bool isDone(void);

private:
	Object* obj;
};

#endif
