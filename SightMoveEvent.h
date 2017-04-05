#ifndef SIGHTMOVEEVENT_H
#define SIGHTMOVEEVENT_H

#include "Event.h"
//this class only exec sight->update()
class Field::SightMoveEvent : public Field::Event
{
public:
	SightMoveEvent(Field*);

	void exec(void);
};

#endif
