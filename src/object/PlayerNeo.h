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

	Vector getSightPointAbsolute(void);
	const Vector& getLookAt(void);
	const Vector& getLookAtN(void);

private:
	Object* rightHand;
	Object* leftHand;
	Object* shoulder;
	Vector sightPoint;
	Vector lookAt;
	Vector lookAtN;
};

#endif
