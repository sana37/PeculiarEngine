#include "Gunner.h"
#include "Object.h"
//#include <iostream>

Gunner::Gunner(const char* fileName, const char* bulletFileName) : Player::Player(fileName), gunsight(Vector(0, -1, 0))
{
	classCode = 'G';
	bulletIndex = 0;
	bulletCount = 0;
	modelBullet = new Object(bulletFileName);
}

Gunner::Gunner(const Gunner& gunner) : Player::Player(gunner)
{
	if (gunner.whichClass() == 'G') {
		bulletCount = gunner.bulletCount;
		bulletIndex = gunner.bulletIndex;
		modelBullet = new Object(*(gunner.modelBullet));
		gunsight.setVector(0, -1, 0);
	} else {
		bulletIndex = 0;
		bulletCount = 0;
	}
}

bool Gunner::updateGunner(void)
{
	this->decelerate();
	if (isDominated == false)
		this->autoMove();
	this->run();

	if (isDominated == false)
		bulletCount++;

	if (bulletCount >= 200) {
		bulletCount = 0;
		return true;
	} else {
		return false;
	}
}

Object Gunner::fire(void)
{
//	bulletIndex++;

	Vector temp = this->getGravityCenter() + (gunsight * 2);
//	Vector temp = this->getGravityCenter();
//	temp.addVector(0, -2, 0);

	modelBullet->moveAbsolute(temp);

	temp = gunsight * 3;
//	temp = gunsight * 0.5;
	modelBullet->setVelocity(temp);

	return Object(*modelBullet);
}

void Gunner::trigger(Vector lookAt)
{
	gunsight = lookAt;
	bulletCount = 200;
}
