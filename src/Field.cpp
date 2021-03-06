#include "Field.h"

#include "Sight.h"
#include "Force.h"
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
	std::cerr << "now, make objects\n";

	object.add(new Object("res/ground0.json"));
	object.add(new Object("res/sky1.json"));
	object.add(new Object("res/dai.json"));
	object.add(new Object("res/hasira.json"));
	object.add(new Object("res/hasira.json"));
	object.add(new Object("res/hasira.json"));
	object.add(new Object("res/hasira.json"));
	object.add(new Object("res/rail.json"));
	object.add(new Object("res/case.json"));

	object[3]->moveRelative(1.19, 0, 1.19);
	object[4]->moveRelative(-1.19, 0, 1.19);
	object[5]->moveRelative(1.19, 0, -1.19);
	object[6]->moveRelative(-1.19, 0, -1.19);
	object[8]->moveRelative(-7.5, 0, 16.50);

	short fixedNum = object.length();

	PlayerNeo* playerNeo = new PlayerNeo("res/playerneo.json");

	object.add(playerNeo);

	object.add(new Object("res/tank.json"));
	object.add(new Object("res/pinpon.json"));
	object.add(new Object("res/pinpon.json"));
	object.add(new Object("res/pinpon.json"));
	object.add(new Object("res/pinpon.json"));
	object.add(new Object("res/pinpon.json"));

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

	object[fixedNum + 1]->moveAbsolute(-7.5, 1, 13.5);

	for (short i = 0; i < 5; i++) {
		object[fixedNum + 2 + i]->moveAbsolute(-6 + 3 * i, 1, 3);
	}

	sight = new Sight(playerNeo, accel, torque);

	forceEvent = new ForceEvent();
	crashEvent = new CrashEvent(playerNeo);
	moveEvent = new MoveEvent();

	time = new QTimer;
	time->start(TIME_UNIT);
	connect(time, SIGNAL(timeout()), this, SLOT(execTimeEvent()));
	QObject::connect(sight, SIGNAL(timeCall()), this, SLOT(timeControl()));

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
	sight->resize(700, 700);
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

	sight->update();
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
	object.add(newObject);
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
