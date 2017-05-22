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

/*
Impulse::Impulse(const Vector& vector, const Vector& point, Object* obj1, Object* obj2) : Force::Force(vector), forcePoint(point), done(true)
{
	this->obj1 = obj1;
	this->obj2 = obj2;

	obj1->getStatus()->attach(obj2);
	obj2->getStatus()->attach(obj1);

	float m1 = obj1->getMass();
	float m2 = obj2->getMass();
	float e = 0.7;
	rv0 = vector / ((1 + e) * (m1 * m2 / (m1 + m2)) * -1.0);//should be in exec() because relative velocity may have changed
}
*/

Impulse::Impulse(const Impulse& impulse) : Force::Force(dynamic_cast<const Force&>(impulse)), forcePoint(impulse.forcePoint), done(impulse.done)
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
	applyForceWithEnergy();
	done = true;
/*
	if (applySmallForce()) {
		std::cerr << "true\n\n";
		done = true;
	} else {
		std::cerr << "false\n\n";
		done = false;
	}
	std::cerr << "obj2 velocity : " << obj2->getVelocity().getMagnitude() << "\n";
*/
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

bool Impulse::applySmallForce(void)
{
	float m1 = obj1->getMass();
	float m2 = obj2->getMass();
	float e = 0.7;
	this->setVector(rv0 * ((1 + e) * (m1 * m2 / (m1 + m2)) * -1.0));

	float impulseSum = 0;
	float impulseMax = this->getMagnitude();
	float dimpulse;
	float relativeGravity = 0;

	float rvg = rv0.getMagnitude() * -1.0;		//relative velocity of gravity point
	float dist = LEAST_DIST; // < |rv0|
	float dx;

	float omega1 = aiueo(*this, obj1);			//if omega > 0, get near. otherwise, get remote
	float omega2 = aiueo(*this * -1.0, obj2);
	float dtheta1;
	float dtheta2;

	int timeDevide = 2;
	float timeLapse = 0;
	float k1 = K1;								//k1 is coefficient of force.  F = k1 / (x^2)
	float k2 = 1 / m1 + 1 / m2;					//(1 / k2) is used as mass by relative velocity
	float k3 = kakikukeko(*this, obj1);			//k3, k4 = r * cos0
	float k4 = kakikukeko(*this, obj2);			// whether + or - does not have relation. kakikukeko returns magnitude.
	float k5 = k3 / obj1->getInertiaMoment();	//k5, k6  =  k3, k4 / I  =  r * cos0 / I
	float k6 = k4 / obj2->getInertiaMoment();

	if (obj1->isFixed()) {
		omega1 = k3 = k5 = 0;

		Vector gravity(0, -GRAVITY, 0);
		relativeGravity = (gravity * (*this)) * -1.0 / this->getMagnitude();

	}
	if (obj2->isFixed()) {
		omega2 = k4 = k6 = 0;

		Vector gravity(0, -GRAVITY, 0);
		relativeGravity = (gravity * (*this)) / this->getMagnitude();

	}

	while (1) {
		std::cerr << dist << ", " << timeLapse << "\n";
		dimpulse = k1 / (dist * dist * timeDevide);	//impulse = F * dt (dt = 1 / timeDevide)
		impulseSum += dimpulse;

		rvg += dimpulse * k2 + relativeGravity / timeDevide;
		omega1 -= dimpulse * k5;					//force applies for objects to get remote. so minus.
		omega2 -= dimpulse * k6;					//omega(t2) - omega(t1)  =  impulse * cos0 * r / I  =  Fcos0 * r * dt / I

		dtheta1 = omega1 / timeDevide;
		dtheta2 = omega2 / timeDevide;

		std::cerr << rvg / timeDevide << " : " << -dtheta1 * k3 << " : " << -dtheta2 * k4 << "\n";
		dx = (rvg / timeDevide) - (dtheta1 * k3) - (dtheta2 * k4);

		if (dx > 0) {
			applyDecomposedForce(obj1, *this);
			applyDecomposedForce(obj2, *this * -1.0);
			return true;
		}

		dist += dx;
		timeLapse += (1.0 / timeDevide);

		if (dist <= 0.0) {							//retry  back status
			timeLapse -= (1.0 / timeDevide);
			dist -= dx;
			omega1 += dimpulse * k5;
			omega2 += dimpulse * k6;
			rvg -= dimpulse * k2 + relativeGravity / timeDevide;
			impulseSum -= dimpulse;
			timeDevide *= 2;
			continue;
		}

		if (timeLapse > 0.99) {
			Vector smallForce = (*this) * (impulseSum / impulseMax);//gravity and air resistance should be considered

			applyDecomposedForce(obj1, smallForce);
			applyDecomposedForce(obj2, smallForce * -1.0);

			(*this) -= smallForce;

			std::cerr << "impulseSum / impulseMax = " << impulseSum / impulseMax << "\n";
			return false;
		}

		if (impulseSum > impulseMax) {
			applyDecomposedForce(obj1, *this);
			applyDecomposedForce(obj2, *this * -1.0);
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