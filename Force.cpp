#include "Force.h"
#include "Object.h"
#include "Calculater.h"
#include <iostream>//to use NULL... I dont like this

Force::Force(Vector vector, Vector point, Object* obj1, Object* obj2) : Vector::Vector(vector), forcePoint(point), permanent(false)
{
	this->obj1 = obj1;
	this->obj2 = obj2;
}

Force::Force(const Force& force) : Vector::Vector((Vector) force), forcePoint(force.forcePoint)
{
	this->obj1 = force.obj1;
	this->obj2 = force.obj2;
	this->permanent = force.permanent;
}

Force::~Force(void)
{
	obj1 = NULL;
	obj2 = NULL;
}

void Force::exec(void)
{
	if (obj1 != NULL) {
		applyDecomposedForce(obj1);
	}

	(*this) *= -1.0;

	if (obj2 != NULL) {
		applyDecomposedForce(obj2);
	}

	(*this) *= -1.0;
}

void Force::applyDecomposedForce(Object* obj)
{
	obj->push(*this);

	Vector baseTrans = (forcePoint - obj->getGravityCenter());
	float radius = baseTrans.getMagnitude();
	baseTrans /= radius;

	Vector transForce = baseTrans * ((*this) * baseTrans);// / (baseTrans * baseTrans);// this is 1.0 !!
	Vector rotateForce = (*this) - transForce;

	Calculater::rotate(&rotateForce, Vector(0, 0, 0), baseTrans, 90.0);
	obj->applyTorque(rotateForce);
}

void Force::setPermanent(bool permanent)
{
	this->permanent = permanent;
}

bool Force::isPermanent(void)
{
	return permanent;
}
