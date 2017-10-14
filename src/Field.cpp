#include "Field.h"

#include "Sight.h"
#include "Force.h"
#include "Impulse.h"
#include "Gravity.h"
#include "UniversalForce.h"
#include "UniversalTorque.h"

#include "Object.h"
#include "PlayerNeo.h"
#include "Gunner.h"

#include "MoveEvent.h"
#include "CrashEvent.h"
#include "ForceEvent.h"

#include "Define.h"

#include <QTimer>
#include <iostream>

Field::Field(void)
{
	std::cerr << "Hello world\n";

	time = new QTimer;

	std::cerr << "now, make objects\n";

	object.add(new Object("res/ground0"));
	object.add(new Object("res/sky1"));
	object.add(new Object("res/dai"));
	object.add(new Object("res/hasira"));
	object.add(new Object("res/hasira"));
	object.add(new Object("res/hasira"));
	object.add(new Object("res/hasira"));
	object.add(new Object("res/rail"));
	object.add(new Object("res/case"));
//	object.add(new Gunner("res/player", "res/bullet"));

	object[3]->moveRelative(1.19, 0, 1.19);
	object[4]->moveRelative(-1.19, 0, 1.19);
	object[5]->moveRelative(1.19, 0, -1.19);
	object[6]->moveRelative(-1.19, 0, -1.19);
	object[8]->moveRelative(-7.5, 0, 16.50);

	short fixedNum = object.length();

	PlayerNeo* playerNeo = new PlayerNeo("res/playerneo");

	object.add(playerNeo);

	object.add(new Object("res/tank"));
	object.add(new Object("res/pinpon"));
	object.add(new Object("res/pinpon"));
	object.add(new Object("res/pinpon"));
	object.add(new Object("res/pinpon"));
	object.add(new Object("res/pinpon"));
//	object.add(new Object("res/object1"));
//	object.add(new Object("res/object2_light"));
//	object.add(new Object("res/object0"));

	std::cerr << "object creation have done.\n";

	for (short i = 0; i < object.length(); i++) {
		if (i < fixedNum) {
			object[i]->fix();
		} else {
			Force* gravity = new Gravity(object[i]);
			addForce(gravity);
		}
	}

	UniversalForce* accel = new UniversalForce(playerNeo);
	UniversalTorque* torque = new UniversalTorque(playerNeo);
	addForce(accel);
	addForce(torque);

	sight = new Sight(playerNeo, accel, torque);

	forceEvent = new ForceEvent();
	crashEvent = new CrashEvent(playerNeo);
	moveEvent = new MoveEvent();

	object[fixedNum + 1]->moveAbsolute(-7.5, 1, 13.5);

	for (short i = 0; i < 5; i++) {
		object[fixedNum + 2 + i]->moveAbsolute(-6 + 3 * i, 1, 3);
	}
/*
	object[fixedNum + 1]->moveAbsolute(8, 25, 0);
//	object[fixedNum + 1]->setVelocity(0.03, 0.05, 0);
	object[fixedNum + 1]->setVelocity(0, 0, 0);
	object[fixedNum + 1]->setOmega(1, 1, 0);
	for (short i = 0; i < 10; i++)
		object[fixedNum + 1]->rotate();
	object[fixedNum + 1]->setOmega(0, 0, 0);

	object[fixedNum + 2]->moveAbsolute(8, 10, 0);
	object[fixedNum + 2]->setVelocity(0, 0, 0);
	object[fixedNum + 2]->setOmega(0.1, 0.1, 0);
	for (short i = 0; i < 1; i++)
		object[fixedNum + 2]->rotate();
	object[fixedNum + 2]->setOmega(0, 0, 0);
*/
	time->start(TIME_UNIT);
	connect(time, SIGNAL(timeout()), this, SLOT(execTimeEvent()));
	QObject::connect(sight, SIGNAL(timeCall()), this, SLOT(timeControl()));

	deadObjectNum = 0;

	std::cerr << "start!\n";
}
/*
Field::Field(const Field& _field) : QObject::QObject()
{
//do nothing because this class make singleton object
}
*/
Field::~Field(void)
{
	sight->close();
}

Field* Field::getInstance(void)
{
	if (field == NULL)
		field = new Field();
	return field;
}

void Field::deleteInstance(void)
{
	if (field != NULL)
		delete field;
	field = NULL;
}

void Field::open(void)
{
	sight->resize(1000, 700);
	sight->move(400, 0);
//	sight->showMaximized();
	sight->show();
}

Object* Field::getObject(int idx)
{
	return object[idx];
}

int Field::getObjectNum(void)
{
	return object.length();
}

void Field::execTimeEvent(void)
{
	forceEvent->execIfEnabled();
	crashEvent->execIfEnabled();
	moveEvent->execIfEnabled();

	sight->updateGL();
}

void Field::timeControl(void)
{
	if (forceEvent->isEnabled()) {
		forceEvent->disable();
		crashEvent->disable();
		moveEvent->disable();
	} else {
		forceEvent->enable();
		crashEvent->enable();
		moveEvent->enable();
	}
}

void Field::addObject(Object* newObject)
{
/*
	if (deadObjectNum > 0) {
		deadObjectNum--;
		delete object[deadObjectIndex[deadObjectNum]];
//		object[deadObjectIndex[deadObjectNum]] = newObject;
		switch (newObject->whichClass()) {
			case 'O' : object[deadObjectIndex[deadObjectNum]] = new Object(*newObject);	break;
			case 'P' : object[deadObjectIndex[deadObjectNum]] = new Player(*((Player*)newObject));	break;
			case 'G' : object[deadObjectIndex[deadObjectNum]] = new Gunner(*((Gunner*)newObject));	break;
			case 'N' : object[deadObjectIndex[deadObjectNum]] = new NumberBox(*((NumberBox*)newObject));	break;
		}
		std::cerr << "Object generation success??\n";
		return;
	}
*/
	object.add(newObject);
//	syncObject();
}

void Field::deleteObject(Object* oldObject)
{
	object.removeIfMatchOnce(oldObject);
	delete oldObject;
}

void Field::addForce(Force* force)
{
	this->force.add(force);
}

void Field::deleteForce(Object* obj)
{
	for (short i = 0; i < force.length(); i++) {
		Gravity* gravity = dynamic_cast<Gravity*>(force[i]);
		if (gravity != NULL) {
			if (gravity->getObject() == obj) {
				force.remove(i);
				delete gravity;
				return;
			}
		}
	}
}

Field* Field::field = NULL;
