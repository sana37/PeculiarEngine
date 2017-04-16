#ifndef CRASHEVENT_H
#define CRASHEVENT_H

#include "Event.h"

class Field::CrashEvent : public Field::Event
{
public:
	CrashEvent(void);

	void exec(void);

private:
	bool interference(short , short);
	char judgeCrash(short , short);
	void reflect(short , short);
	bool calculate1(float* , float* , float*);

};

#endif
