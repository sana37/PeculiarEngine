#include "ObjectStatus.h"
#include "Object.h"
#include "Define.h"

ObjectStatus::ObjectStatus(void) : attachedObjects(FIRST_ATTACH_MAX), readyCrash(false)
{
}

ObjectStatus::ObjectStatus(const ObjectStatus& status) : attachedObjects(status.attachedObjects), readyCrash(status.readyCrash)
{
}

const Array<Object*>& ObjectStatus::getAttachedObjects(void)
{
	return attachedObjects;
}

void ObjectStatus::attach(Object* object)
{
	attachedObjects.add(object);
}

void ObjectStatus::detach(Object* object)
{
	short objectNum = attachedObjects.length();

	for (short i = 0  ;  i < objectNum  ;  i++) {
		if (attachedObjects[i] == object) {
			attachedObjects.remove(i);
			break;
		}
	}
}

bool ObjectStatus::isReadyCrash(void)
{
	return readyCrash;
}

void ObjectStatus::setReadyCrash(void)
{
	readyCrash = true;
}

void ObjectStatus::setDoneCrash(void)
{
	readyCrash = false;
}
