#ifndef PLAYERNEO_H
#define PLAYERNEO_H

#include "Object.h"
#include "Array.h"

class Vector;

class PlayerNeo : public Object
{
public:
	explicit PlayerNeo(const char*);
	PlayerNeo(const PlayerNeo&);
	virtual ~PlayerNeo(void);

	virtual void update(void);
	virtual char whichClass(void);

	void moveShoulder(float);
	void hold(float);

	Vector getSightPoint(void);
	Vector getSightPointN(void);
	const Vector& getLookAt(void);
	const Vector& getLookAtN(void);
	Vector getLeftHandRelativeVelocity(void);
	Vector getRightHandRelativeVelocity(void);
	Vector getShoulderRelativeVelocity(void);

	Vector getDeltaVertex(short);
	Vector getDeltaPolygon(short);
	Vector getDeltaLine(short);

	bool isLeftHand(short);
	bool isRightHand(short);
	bool isShoulderOrHeldObject(short);
	bool isLeftHandL(short);
	bool isRightHandL(short);
	bool isShoulderOrHeldObjectL(short);
	bool isLeftHandV(short);
	bool isRightHandV(short);
	bool isShoulderOrHeldObjectV(short);

	void initializeTouchState(void);
	void addLeftObject(Object*);
	void addRightObject(Object*);
	Object* getHoldableObject(void);

	void setHold(const char*);
	bool holds(void);

private:
	Vector leftStartIdx;
	Vector rightStartIdx;
	Vector shoulderStartIdx;
	Vector caughtObjStartIdx;

	Vector leftHandDirection;
	Vector shoulderDirection;

	float leftHandVelocity;
	float shoulderVelocity;

	float leftPos;
	float rightPos;
	float shoulderPos;

	Vector sightPoint;
	Vector sightPointN;
	Vector lookAt;
	Vector lookAtN;

	bool holdFlag;
	const char* caughtObjName;
	Array<Object*> leftTouchObjects;
	Array<Object*> rightTouchObjects;
};

#endif
