#include "Gravity.h"
#include "Object.h"
#include "Define.h"
#include <iostream>

Gravity::Gravity(Object* obj) : Force::Force(Vector(0, -GRAVITY, 0) * obj->getMass())
{
	this->obj = obj;
}

Gravity::Gravity(const Gravity& gravity) : Force::Force(dynamic_cast<const Force&>(gravity))
{
	this->obj = gravity.obj;
}

Gravity::~Gravity(void)
{
	obj = NULL;
}

void Gravity::exec(void)
{
	obj->push(*this);
}

bool Gravity::isDone(void)
{
	return false;
}
