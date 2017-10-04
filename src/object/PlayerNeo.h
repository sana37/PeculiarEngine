#ifndef PLAYERNEO_H
#define PLAYERNEO_H

#include "Object.h"

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

	Vector getDeltaVertex(short);
	Vector getDeltaPolygon(short);
	Vector getDeltaLine(short);

private:
	Vector leftStartIdx;
	Vector rightStartIdx;
	Vector shoulderStartIdx;

	Vector leftHandDirection;
	Vector shoulderDirection;

	float leftHandVelocity;
	float shoulderVelocity;

	Vector sightPoint;
	Vector sightPointN;
	Vector lookAt;
	Vector lookAtN;
};

#endif
