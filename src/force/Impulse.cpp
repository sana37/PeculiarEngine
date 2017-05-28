#include "Impulse.h"
#include "Object.h"
#include "ObjectStatus.h"
#include "Calculater.h"
#include "Define.h"
#include <iostream>//to use NULL... I dont like this


Impulse::Impulse(const Vector& base, const Vector& point, Object* obj1, Object* obj2) : Force::Force(Vector()), forcePoint(point), done(true)
{
	this->obj1 = obj1;
	this->obj2 = obj2;
	this->base = base;

	obj1->getStatus()->setReadyCrash();
	obj2->getStatus()->setReadyCrash();
//	obj1->getStatus()->attach(obj2);
//	obj2->getStatus()->attach(obj1);
}

Impulse::Impulse(const Impulse& impulse) : Force::Force(dynamic_cast<const Force&>(impulse)), forcePoint(impulse.forcePoint), done(impulse.done)
{
	this->obj1 = impulse.obj1;
	this->obj2 = impulse.obj2;
	this->base = impulse.base;
}

Impulse::~Impulse(void)
{// if multiple objects crash the object, setdonecrash release all crash.  this is bad.  modify later.  attach can be used efficiently
	obj1->getStatus()->setDoneCrash();
	obj2->getStatus()->setDoneCrash();
//	obj1->getStatus()->detach(obj2);
//	obj2->getStatus()->detach(obj1);

	obj1 = NULL;
	obj2 = NULL;
}

void Impulse::exec(void)
{
	applyForceWithEnergy();
	done = true;
}

bool Impulse::isDone(void)
{
	return done;
}

void Impulse::applyForceWithEnergy(void)
{
	Vector v1 = obj1->getVelocity();
	Vector v2 = obj2->getVelocity();
	Vector w1 = obj1->getOmega();
	Vector w2 = obj2->getOmega();
	Vector r1 = forcePoint - obj1->getGravityCenter();
	Vector r2 = forcePoint - obj2->getGravityCenter();
	Vector edv = ((v2 - v1) * base >= 0) ? base : (base * -1.0);
	Vector edw1 = (edv % r1) / -(r1.getMagnitude());
	Vector edw2 = (edv % r2) / r2.getMagnitude();
	float e = 0.8;
	v1 *= e;
	v2 *= e;
	w1 *= e;
	w2 *= e;
	float m1 = obj1->getMass();
	float m2 = obj2->getMass();
	float i1 = obj1->getInertiaMoment();
	float i2 = obj2->getInertiaMoment();
	float rcos01 = kakikukeko(edv, obj1);
	float rcos02 = kakikukeko(edv, obj2);
	float F = -2.0 * ((v1*edv) + rcos01*(w1*edw1) - (v2*edv) + rcos02*(w2*edw2)) / (1/m1 + rcos01*rcos01/i1 + 1/m2 + rcos02*rcos02/i2);

	obj1->push(edv * F);
	obj2->push(edv * -F);
	obj1->applyTorque(edw1 * (F * rcos01));
	obj2->applyTorque(edw2 * (F * rcos02));
}

void Impulse::applyDecomposedForce(Object* obj, const Vector& force)
{
	if (obj->isFixed())
		return;

	obj->push(force);

	Vector baseTrans = (forcePoint - obj->getGravityCenter());
	float radius = baseTrans.getMagnitude();
	baseTrans /= radius;

	Vector transForce = baseTrans * (force * baseTrans);
	Vector rotateForce = force - transForce;

	obj->applyTorque(baseTrans % rotateForce * radius);
}

float Impulse::kakikukeko(const Vector& force, Object* obj)
{
	Vector baseTrans = (forcePoint - obj->getGravityCenter());
	float radius = baseTrans.getMagnitude();
	baseTrans /= radius;

	Vector transForce = baseTrans * (force * baseTrans);
	Vector rotateForce = force - transForce;

	float cos0 = (force * rotateForce) / (force.getMagnitude() * rotateForce.getMagnitude());

	if (cos0 >= 0)
		return cos0 * radius;
	else
		return -cos0 * radius;
}

Vector Impulse::getForcePoint(void)
{
	return forcePoint;
}
