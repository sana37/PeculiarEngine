#ifndef GRAVITY_H
#define GRAVITY_H

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
