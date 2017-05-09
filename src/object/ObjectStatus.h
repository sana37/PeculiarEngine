#ifndef OBJECTSTATUS_H
#define OBJECTSTATUS_H

#define FIRST_ATTACH_MAX	10

#include "Array.h"

class Object;

class ObjectStatus
{
	friend class Object;

private:
	ObjectStatus(void);
	ObjectStatus(const ObjectStatus&);

public:
	const Array<Object*>& getAttachedObjects(void);
	void attach(Object*);
	void detach(Object*);

private:
	Array<Object*> attachedObjects;

};

#endif
