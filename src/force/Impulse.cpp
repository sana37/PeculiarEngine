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

	obj1->getStatus()->attach(obj2);
	obj2->getStatus()->attach(obj1);
}

Impulse::Impulse(const Impulse& impulse) : Force::Force(dynamic_cast<const Force&>(impulse)), forcePoint(impulse.forcePoint), done(impulse.done)
{
	this->obj1 = impulse.obj1;
	this->obj2 = impulse.obj2;
	this->base = impulse.base;
}

Impulse::~Impulse(void)
{
	obj1->getStatus()->detach(obj2);
	obj2->getStatus()->detach(obj1);

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
	float m1 = obj1->getMass();
	float m2 = obj2->getMass();
	float m = m1 / m2;
	float i1 = obj1->getInertiaMoment();
	float i2 = obj2->getInertiaMoment();
	float rcos01 = kakikukeko(edv, obj1);
	float rcos02 = kakikukeko(edv, obj2);
	float k1 = rcos01 * m1 / i1;			//k1, k2 = r * cos0 * m / I
	float k2 = rcos02 * m2 / i2;			// whether + or - does not have relation. kakikukeko returns magnitude.
	float dv1Abs = -2.0 * m1 * ((v1*edv) + rcos01*(w1*edw1) - (v2*edv) + rcos02*(w2*edw2)) / (m1 + i1*k1*k1 + m2*m*m + i2*k2*k2*m*m);

	Vector dv1 = edv * dv1Abs;
	Vector dv2 = dv1 * (-1.0 * m);
	Vector dw1 = edw1 * (k1 * dv1Abs);
	Vector dw2 = edw2 * (k2 * dv1Abs * m);

	obj1->push(dv1 * m1);
	obj2->push(dv2 * m2);
	obj1->applyTorque(dw1 * i1);
	obj2->applyTorque(dw2 * i2);
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

/*
Vector Impulse::getForcePoint(void)
{
	return forcePoint;
}
*/