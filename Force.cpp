#include "Force.h"
#include "Object.h"
#include <iostream>//to use NULL... I dont like this

Force::Force(Vector vector, Object* obj1, Object* obj2) : Vector::Vector(vector)
{
	this->obj1 = obj1;
	this->obj2 = obj2;
}

Force::~Force(void)
{
	obj1 = NULL;
	obj2 = NULL;
}

void Force::exec(void)
{
	obj1->push(*this);
	obj2->push((*this) * -1);
}
