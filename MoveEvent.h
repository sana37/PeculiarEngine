#ifndef MOVEEVENT_H
#define MOVEEVENT_H

#include "Event.h"

class Field::MoveEvent : public Field::Event
{
public:
	MoveEvent(Field*);

	void exec(void);
};

#endif
