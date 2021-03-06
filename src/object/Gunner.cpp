#include "Gunner.h"
#include "Object.h"
#include "Field.h"
//#include <iostream>

Gunner::Gunner(const char* fileName, const char* bulletFileName) :
	Player::Player(fileName), gunsight(Vector(0, -1, 0))
{
//	classCode = 'G';
	bulletIndex = 0;
	bulletCount = 0;
	modelBullet = new Object(bulletFileName);
}

Gunner::Gunner(const Gunner& gunner) : Player::Player(gunner)
{
	bulletCount = gunner.bulletCount;
	bulletIndex = gunner.bulletIndex;
	modelBullet = new Object(*(gunner.modelBullet));
	gunsight = gunner.gunsight;
}

void Gunner::update(void)
{
	this->decelerate();
	if (isDominated == false)
		this->autoMove();
	this->run();

	if (isDominated == false)
		bulletCount++;

	if (bulletCount >= 200) {
		bulletCount = 0;
		Object* bullet = fire();
		Field::getInstance()->addObject(bullet);
	}
}

Object* Gunner::fire(void)
{
//	bulletIndex++;

	Vector temp = this->getGravityCenter() + (gunsight * 2);

	modelBullet->moveAbsolute(temp);

//	temp = gunsight * 3;
	temp = gunsight * 0.5;
	modelBullet->setVelocity(temp);

	return new Object(*modelBullet);
}

void Gunner::trigger(Vector lookAt)
{
	gunsight = lookAt;
	bulletCount = 200;
}

void Gunner::rotateBullet(Vector omega)
{
	modelBullet->setOmega(omega);
	modelBullet->rotate();
	modelBullet->setOmega(0, 0, 0);
}

char Gunner::whichClass(void)
{
	return 'G';
}
