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
	void execFirstCrash(void);
	void execSecondCrash(void);
	bool canCrashObjSphere(Object*, Object*);
	bool canCrashObjSphereAndVrtx(Object*, Vector);
	int reflectIfCrash(Object*, Object*);
	bool resolveCaught(Object*, Object*);
	Vector calcCaughtDist(Object*, Object*);
	Vector getLineToPolygonPenetration1(Object*, const Vector&, short, float);
	Vector getLineToPolygonPenetration2(Object*, Object*, short, short, short);
	void judgePlgnAndVrtx(Object*, Object*, CrashResult*);
	void judgeLineAndLine(Object*, Object*, CrashResult*);
	void reflectPlgnAndVrtx(CrashResult*);
	void reflectLineAndLine(CrashResult*);
	void calcRepulsion(Object*, Object*, const Vector&, const Vector&, CrashResult*);

	CrashKeeper* crashKeeper;
};

#endif
