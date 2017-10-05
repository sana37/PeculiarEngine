#include "UniversalTorque.h"
#include "Object.h"
#include "Define.h"
#include <iostream>

UniversalTorque::UniversalTorque(Object* obj) : Force::Force(Vector(0, 0, 0))
{
	this->object = obj;
}

UniversalTorque::UniversalTorque(const UniversalTorque& uniTorque) : Force::Force(dynamic_cast<const Force&>(uniTorque))
{
	this->object = uniTorque.object;
}

UniversalTorque::~UniversalTorque(void)
{
	this->object = NULL;
}

void UniversalTorque::exec(void)
{
	if (object->getOmega().getMagnitude() < OMEGA_MAX)
		object->applyTorque(*this);
}

bool UniversalTorque::isDone(void)
{
	return false;
}

Vector UniversalTorque::getForcePoint(void)
{
	return this->object->getGravityCenter();
}

void UniversalTorque::changeObject(Object* obj)
{
	this->object = obj;
}
