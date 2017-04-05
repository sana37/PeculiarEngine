#ifndef EVENT_H
#define EVENT_H

#include "Field.h"

class Field::Event
{
public:
	Event(Field*);

	virtual void exec(void) = 0;
	void enable(void);
	void disable(void);
	bool isEnabled(void);

protected:
	Field* field;

private:
	bool execFlag;
};

#endif
