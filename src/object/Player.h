#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

class Vector;

class Player : public Object
{
public:
	explicit Player(const char*);
	Player(const Player&);

	virtual void update(void);
	char whichClass(void);

protected:
	void decelerate(void);
	void autoMove(void);

private:
	Vector* route;
	short routeNum;
	short routeIndex;
};

#endif
