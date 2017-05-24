#ifndef STICKFORCE_H
#define STICKFORCE_H

#include "Force.h"
#include "CrashResult.h"

class Object;

class StickForce : public Force
{
public:
	StickForce(const Vector&, Object*, Object*, const Field::CrashEvent::CrashResult&);
	StickForce(const StickForce&);
	virtual ~StickForce(void);

	void exec(void);
	bool isDone(void);

	void applyStaticForce(Object*, Object*);
	void applyDecomposedForce(Object*, const Vector&, const Vector&);
	Vector getForcePoint(void);

private:
	Object* obj1;
	Object* obj2;
	Vector base;
//	Vector forcePoint;
	Field::CrashEvent::CrashResult result;
	bool done;
};

#endif
