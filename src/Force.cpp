#include "Force.h"
#include "Object.h"
#include "ObjectStatus.h"
#include "Calculater.h"
#include <iostream>//to use NULL... I dont like this

Force::Force(Vector vector, Vector point, Object* obj1, Object* obj2) : Vector::Vector(vector), forcePoint(point), permanent(false), setted(true)
{
	this->obj1 = obj1;
	this->obj2 = obj2;
	if (obj1 != NULL  &&  obj2 != NULL) {	//i want to make force classes else
		obj1->getStatus()->attach(obj2);
		obj2->getStatus()->attach(obj1);
	}
}

Force::Force(const Force& force) : Vector::Vector((Vector) force), forcePoint(force.forcePoint)
{
	this->obj1 = force.obj1;
	this->obj2 = force.obj2;
	this->permanent = force.permanent;
}

Force::~Force(void)
{
	if (obj1 != NULL  &&  obj2 != NULL) {
		obj1->getStatus()->detach(obj2);
		obj2->getStatus()->detach(obj1);
	}
	obj1 = NULL;
	obj2 = NULL;
}

void Force::exec(void)
{
	if (obj1 == NULL  &&  obj2 != NULL) {
//		forcePoint = obj2->getGravityCenter();
//		(*this) *= -1.0;
		obj2->push(*this);
		return;
	}
	if (obj1 != NULL  &&  obj2 == NULL) {
//		forcePoint = obj1->getGravityCenter();
		obj1->push(*this);
		return;
	}

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

//	Calculater::rotate(&rotateForce, Vector(0, 0, 0), baseTrans, 90.0);
	Calculater::rotateRad(&rotateForce, Vector(0, 0, 0), baseTrans, 3.141592 / 2);
	obj->applyTorque(rotateForce * radius);
}

void Force::setPermanent(bool permanent)
{
	this->permanent = permanent;
}

void Force::enSet(void)
{
	setted = true;
}

void Force::disSet(void)
{
	setted = false;
}

bool Force::isPermanent(void)
{
	return permanent;
}

bool Force::isSetted(void)
{
	return setted;
}

Vector Force::getForcePoint(void)
{
	return forcePoint;
}
