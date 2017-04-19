#ifndef CRASHEVENT_H
#define CRASHEVENT_H

#include "Event.h"

class Object;

class Field::CrashEvent : public Field::Event
{
public:
	class CrashResult;

public:
	CrashEvent(void);

	void exec(void);

private:
	bool interference(Object*, Object*);
	void judgeCrash(Object*, Object*, char*, short*, short*);
	CrashResult judgePlgnAndVrtx(Object*, Object*, short*, short*, bool);
	CrashResult judgeLineAndLine(Object*, Object*, short*, short*, bool);
	void reflect(Object*, Object*, char, short, short);
	bool calculate1(float*, float*, float*);

	static const char OVER;
	static const char SUCCESS;
	static const char FAILURE;
};

#endif
