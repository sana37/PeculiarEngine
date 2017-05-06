#ifndef FORCEEVENT_H
#define FORCEEVENT_H

#include "Event.h"

class Field::ForceEvent : public Field::Event
{
public:
	ForceEvent(void);

	void exec(void);
};

#endif
