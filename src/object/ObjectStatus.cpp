#include "ObjectStatus.h"

ObjectStatus::ObjectStatus(void) : floating(true)
{
}

ObjectStatus::ObjectStatus(const ObjectStatus& status)
{
	floating = status.floating;
}

bool ObjectStatus::isFloat(void)
{
	return floating;
}

void ObjectStatus::attach(void)
{
	floating = false;
}

void ObjectStatus::detach(void)
{
	floating = true;
}
