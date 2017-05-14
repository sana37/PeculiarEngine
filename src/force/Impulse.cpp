#include "Impulse.h"
#include "Object.h"
#include "ObjectStatus.h"
#include "Calculater.h"
#include <iostream>//to use NULL... I dont like this
/*
Impulse::Impulse(Vector rv, Vector point, Object* obj1, Object* obj2) :
	Force::Force(Vector(0, 0, 0)), //fake
	rv0(rv),
	forcePoint(point)
{
	this->obj1 = obj1;
	this->obj2 = obj2;

	obj1->getStatus()->attach(obj2);
	obj2->getStatus()->attach(obj1);

//	setup();
///
	float m1 = obj1->getMass();
	float m2 = obj2->getMass();
	float e = 0.7;
	this->setVector(rv0 * (1 + e) * (m1 * m2 / (m1 + m2)) * -1.0);
///
}
*/
Impulse::Impulse(const Vector& vector, const Vector& point, Object* obj1, Object* obj2) : Force::Force(vector), forcePoint(point)
{
	this->obj1 = obj1;
	this->obj2 = obj2;

	obj1->getStatus()->attach(obj2);
	obj2->getStatus()->attach(obj1);

	float m1 = obj1->getMass();
	float m2 = obj2->getMass();
	float e = 0.7;
	rv0 = vector / ((1 + e) * (m1 * m2 / (m1 + m2)) * -1.0);
}

Impulse::Impulse(const Impulse& impulse) : Force::Force(dynamic_cast<const Force&>(impulse)), forcePoint(impulse.forcePoint)
{
	this->obj1 = impulse.obj1;
	this->obj2 = impulse.obj2;
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

	applyDecomposedForce(obj1);

	(*this) *= -1.0;

	applyDecomposedForce(obj2);

	(*this) *= -1.0;

}

void Impulse::applyDecomposedForce(Object* obj)
{
	obj->push(*this);

	Vector baseTrans = (forcePoint - obj->getGravityCenter());
	float radius = baseTrans.getMagnitude();
	baseTrans /= radius;

	Vector transForce = baseTrans * ((*this) * baseTrans);// / (baseTrans * baseTrans);// this is 1.0 !!
	Vector rotateForce = (*this) - transForce;

//	Calculater::rotateDeg(&rotateForce, Vector(0, 0, 0), baseTrans, 90.0);
//	obj->applyTorque(rotateForce);
	obj->applyTorque(baseTrans % rotateForce * radius);
}

bool Impulse::applySmallForce(void)//Object* obj)
{
	float m1 = obj1->getMass();
	float m2 = obj2->getMass();
	float e = 0.7;
	this->setVector(rv0 * ((1 + e) * (m1 * m2 / (m1 + m2)) * -1.0));

	float impulseSum = 0;
	float impulseMax = this->getMagnitude();
	float dimpulse;

	float rvg = rv0.getMagnitude() * -1.0;		//relative velocity of gravity point
	float dist = LEAST_DIST; // < |rv0|
	float dx;

	float omega1 = aiueo(*this, obj1);			//if omega > 0, get near. otherwise, get remote
	float omega2 = aiueo(*this * -1.0, obj2);
//	float theta1 = 0;
//	float theta2 = 0;
	float dtheta1;
	float dtheta2;

	int timeDevide = 1;
	float timeLapse = 0;
	float k1 = K1;								//k1 is coefficient of force.  F = k1 / (x^2)
	float k2 = 1 / m1 + 1 / m2;					//(1 / k2) is used as mass by relative velocity
	float k3 = kakikukeko(*this, obj1);			//k3, k4 = r * cos0
	float k4 = kakikukeko(*this, obj2);			// whether + or - does not have relation. kakikukeko returns magnitude.
	float k5 = k3 / obj1->getInertiaMoment();	//k5, k6  =  k3, k4 / I  =  r * cos0 / I
	float k6 = k4 / obj2->getInertiaMoment();

	while (1) {
		dimpulse = k1 / (dist * dist * timeDevide);	//impulse = F * dt (dt = 1 / timeDevide)
		impulseSum += dimpulse;

		rvg += dimpulse * k2;
		omega1 -= dimpulse * k5;					//force applies for objects to get remote. so minus.
		omega2 -= dimpulse * k6;					//omega(t2) - omega(t1)  =  impulse * cos0 * r / I  =  Fcos0 * r * dt / I

		dtheta1 = omega1 / timeDevide;
		dtheta2 = omega2 / timeDevide;
//		theta1 += dtheta1;
//		theta2 += dtheta2;

		dx = (rvg / timeDevide) - (dtheta1 * k3) - (dtheta2 * k4);

		if (dx > 0)
			return true;

		dist += dx;
		timeLapse += (1 / timeDevide);

		if (timeLapse > 0.99)
			return false;

		if (dist <= 0.0) {							//retry  back status
			timeLapse -= (1 / timeDevide);
			dist -= dx;
//			theta1 -= dtheta1;
//			theta2 -= dtheta2;
			omega1 += dimpulse * k5;
			omega2 += dimpulse * k6;
			rvg -= dimpulse * k2;
			impulseSum -= dimpulse;
			timeDevide *= 2;
		}

		if (impulseSum > impulseMax) {
			std::cerr << "impulse error or rubbed\n";
			return true;
		}
	}

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

//float Impulse::aiueo(const Vector& omega, const Vector& force, const Vector& radius)
float Impulse::aiueo(const Vector& force, Object* obj)
{
	Vector omega = obj->getOmega();
	Vector radius = (forcePoint - obj->getGravityCenter());
	Vector n = force % radius;
	Vector solution;
	if (Calculater::solveCubicEquation(force, radius, n, omega, &solution)) {
//		Vector partOfOmega = n * solution.getZ();
//		return partOfOmega.getMagnitude();
		return n.getMagnitude() * solution.getZ();//solution.getZ() 'S + or - is important
	} else {
		std::cerr << "cannot extract part of omega\n";
		return -1;
	}
}

/*
Vector Impulse::getForcePoint(void)
{
	return forcePoint;
}
*/