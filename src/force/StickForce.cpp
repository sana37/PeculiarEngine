#include "StickForce.h"
#include "Object.h"
#include "Define.h"

StickForce::StickForce(const Vector& base, Object* obj1, Object* obj2, const Field::CrashEvent::CrashResult& result) :
	Force::Force(Vector()),
	base(base),
//	forcePoint(forcePoint),
	result(result),
	done(false)
{
	this->obj1 = obj1;
	this->obj2 = obj2;
	std::cerr << "stick start.\n";
	if (obj1->isFixed() == true  &&  obj2->isFixed() == false) {
		obj2->back();
		obj2->accelerate(base * -(obj2->getVelocity() * base));
	} else if (obj1->isFixed() == false  &&  obj2->isFixed() == true) {
		obj1->back();
		obj1->accelerate(base * -(obj1->getVelocity() * base));
	} else {
		std::cerr << "undefined.\n";
	}
}

StickForce::StickForce(const StickForce& stickForce) : Force::Force(Vector())
{
	this->setVector(dynamic_cast<const Vector&>(stickForce));
	this->obj1 = stickForce.obj1;
	this->obj2 = stickForce.obj2;
	this->base = stickForce.base;
//	this->forcePoint = stickForce.forcePoint;
	this->result = stickForce.result;
	this->done = stickForce.done;
}

StickForce::~StickForce(void)
{
	std::cerr << "stick end.\n";
	obj1 = NULL;
	obj2 = NULL;
}

void StickForce::exec(void)
{
	if (obj1->isFixed() == true  &&  obj2->isFixed() == false) {
		applyStaticForce(obj1, obj2);
	} else if (obj1->isFixed() == false  &&  obj2->isFixed() == true) {
		applyStaticForce(obj2, obj1);
	} else {
		std::cerr << "undefined.\n";
	}
	done = true;
}

bool StickForce::isDone(void)
{
	return done;
}

/*
** this function can be called only when "gatagatasitekuttuku"
*/
void StickForce::applyStaticForce(Object* attachedObj, Object* detachedObj)
{
//	float relativeAcceleration = (detachedObj->getAcceleration() - attachedObj->getAcceleration()) * base;
////// kimotiwarui
	float relativeAcceleration = Vector(0, -GRAVITY, 0) * base;

	if (relativeAcceleration < -ZERO_ACCELERATION  ||  ZERO_ACCELERATION < relativeAcceleration) {
//		std::cerr << "static force is executed.\n";
		Vector force = base * (detachedObj->getMass() * -relativeAcceleration);
		applyDecomposedForce(detachedObj, force, result.getCrashSpot());
		applyDecomposedForce(attachedObj, force * -1.0, result.getCrashSpot());
//		detachedObj->push(force);
//		attachedObj->push(force * -1.0);
		Vector velocity = detachedObj->getVelocity();
//		std::cerr << velocity.getX() << " : " << velocity.getY() << " : " << velocity.getZ() << "\n";
	} else {
		std::cerr << "static force is ignored.\n";
	}
////// kimotiwarui
}

void StickForce::applyDecomposedForce(Object* obj, const Vector& force, const Vector& forcePoint)
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

Vector StickForce::getForcePoint(void)
{
	return result.getCrashSpot();
}
