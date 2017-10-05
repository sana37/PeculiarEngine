#include "UniversalForce.h"
#include "Object.h"
#include "Define.h"
#include <iostream>

UniversalForce::UniversalForce(Object* obj) : Force::Force(Vector(0, 0, 0))
{
	this->object = obj;
}

UniversalForce::UniversalForce(const UniversalForce& uniForce) : Force::Force(dynamic_cast<const Force&>(uniForce))
{
	this->object = uniForce.object;
}

UniversalForce::~UniversalForce(void)
{
	this->object = NULL;
}

void UniversalForce::exec(void)
{
	if (object->getVelocity().getMagnitude() < SPEED_MAX)
		object->push(*this);
}

bool UniversalForce::isDone(void)
{
	return false;
}

Vector UniversalForce::getForcePoint(void)
{
	return this->object->getGravityCenter();
}

void UniversalForce::changeObject(Object* obj)
{
	this->object = obj;
}
