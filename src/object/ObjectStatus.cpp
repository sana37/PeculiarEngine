#include "ObjectStatus.h"
#include "Object.h"

ObjectStatus::ObjectStatus(void) : attachedObjects(FIRST_ATTACH_MAX)
{
}

ObjectStatus::ObjectStatus(const ObjectStatus& status) : attachedObjects(status.attachedObjects)
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
