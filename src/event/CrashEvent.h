#ifndef CRASHEVENT_H
#define CRASHEVENT_H

#include "Event.h"

class Vector;
class Object;
class CrashKeeper;

class Field::CrashEvent : public Field::Event
{
public:
	class CrashResult;

public:
	CrashEvent(void);

	void exec(void);

private:
	bool canCrashObjSphere(Object*, Object*);
	bool canCrashObjSphereAndVrtx(Object*, Vector);
	bool reflectIfCrash(Object*, Object*);
	void judgePlgnAndVrtx(Object*, Object*, CrashResult*);
	void judgeLineAndLine(Object*, Object*, CrashResult*);
	void reflectPlgnAndVrtx(Object*, Object*, CrashResult*);
	void reflectLineAndLine(Object*, Object*, CrashResult*);
	void calcRepulsion(Object*, Object*, const Vector&, const Vector&, CrashResult*);

	static const char OVER;
	static const char SUCCESS;
	static const char FAILURE;

	CrashKeeper* crashKeeper;
};

#endif
