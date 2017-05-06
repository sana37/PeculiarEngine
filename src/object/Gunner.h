#ifndef GUNNER_H
#define GUNNER_H

#include "Player.h"

class Object;

class Gunner : public Player
{
public:
	Gunner(const char*, const char*);
	Gunner(const Gunner&);

	bool updateGunner(void);
	Object* fire(void);
	void trigger(Vector);
	void rotateBullet(Vector);
	char whichClass(void);

private:
	Object* modelBullet;
	Vector gunsight;
	short bulletIndex;
	int bulletCount;
};

#endif
