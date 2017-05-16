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
	virtual bool isDone(void) = 0;

};

#endif
