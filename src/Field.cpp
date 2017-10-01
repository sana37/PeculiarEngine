#include "Field.h"

#include "Sight.h"
#include "Force.h"
#include "Impulse.h"
#include "Gravity.h"

#include "Object.h"
#include "Player.h"
#include "NumberBox.h"
#include "Gunner.h"

#include "MoveEvent.h"
#include "SightMoveEvent.h"
#include "CrashEvent.h"
#include "ForceEvent.h"

#include "CrashKeeper.h"////
#include "Define.h"

#include <QTimer>
#include <iostream>

Field::Field(void)
{
	std::cerr << "Hello world\n";

	time = new QTimer;
	autoGeneration = new QTimer;

	std::cerr << "now, make objects\n";

	object.add(new Object("res/ground0"));
	object.add(new Object("res/sky1"));
	object.add(new Gunner("res/player", "res/bullet"));

	object.add(new Object("res/object2"));
//	object.add(new Object("res/object2"));
//	object.add(new Object("res/object1"));
//	object.add(new Object("res/object0"));

	std::cerr << "object creation have done.\n";

	sight = new Sight(&object, 2, &force);

	CrashKeeper::getInstance(&object);

	sightMoveEvent = new SightMoveEvent();
	forceEvent = new ForceEvent();
	crashEvent = new CrashEvent();
	moveEvent = new MoveEvent();

///
	for (short i = 3; i < object.length(); i++) {
		Force* gravity = new Gravity(object[i]);
		addForce(gravity);
	}
///

	object[0]->fix();
	object[1]->fix();

	object[3]->moveAbsolute(8, 25, 0);
//	object[3]->setVelocity(0.03, 0.05, 0);
	object[3]->setVelocity(0, 0, 0);
	object[3]->setOmega(1, 1, 0);
	for (short i = 0; i < 10; i++)
		object[3]->rotate();
	object[3]->setOmega(0, 0, 0);
/*
	object[4]->moveAbsolute(8, 10, 0);
	object[4]->setVelocity(0, 0, 0);
	object[4]->setOmega(0.1, 0.1, 0);
	for (short i = 0; i < 1; i++)
		object[4]->rotate();
	object[4]->setOmega(0, 0, 0);
*/
/*
	object[5]->moveAbsolute(3, 13, 3);
//	object[5]->setVelocity(0, -0.05, 0.03);
	object[5]->setVelocity(0, 0, 0);
	object[5]->setOmega(0.1, 0.1, 0);
	object[5]->rotate();
	object[5]->setOmega(0, 0, 0);

	object[6]->moveAbsolute(0, 15, 0);
//	object[6]->setVelocity(0.04, 0.1, 0);
	object[6]->setVelocity(0, 0, 0);
*/

	time->start(TIME_UNIT);
	autoGeneration->start(5000);
	connect(time, SIGNAL(timeout()), this, SLOT(execTimeEvent()));
	QObject::connect(sight, SIGNAL(timeCall()), this, SLOT(timeControl()));
//	QObject::connect(autoGeneration, SIGNAL(timeout()), this, SLOT(autoGenerate()));

	deadObjectNum = 0;
	autoGenerationIndex = 0;
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

void Field::execTimeEvent(void)
{
	sightMoveEvent->execIfEnabled();
	forceEvent->execIfEnabled();
	crashEvent->execIfEnabled();
	moveEvent->execIfEnabled();

	sight->updateGL();
}

void Field::timeControl(void)
{
//sightMoveEvent is always enabled
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

void Field::autoGenerate(void)
{
	Object* temp;
	Vector vector;

	switch (autoGenerationIndex) {//atode random ni suru
		case 0 : {
			temp = new NumberBox(94);
			break;
		}
		case 1 : {
			temp = new NumberBox(42);
			break;
		}
		case 2 : {
			temp = new NumberBox(66);
			break;
		}
		case 3 : {
			temp = new NumberBox(81);
			break;
		}
		case 4 : {
			temp = new NumberBox(12);
			break;
		}
		case 5 : {
			temp = new NumberBox(35);
			break;
		}
		case 6 : {
			temp = new NumberBox(48);
			break;
		}
		case 7 : {
			temp = new NumberBox(18);
			break;
		}
		case 8 : {
			temp = new NumberBox(99);
			break;
		}
		case 9 : {
			temp = new NumberBox(6);
			break;
		}
		case 10 : {
			temp = new NumberBox(68);
			break;
		}
		case 11 : {
			temp = new NumberBox(32);
			break;
		}
		default : {
			return;
		}
	}

	switch (autoGenerationIndex % 4) {
		case 0 : {
			vector.setVector(25, 45, 25);
			break;
		}
		case 1 : {
			vector.setVector(25, 45, -25);
			break;
		}
		case 2 : {
			vector.setVector(-25, 45, -25);
			break;
		}
		case 3 : {
			vector.setVector(-25, 45, 25);
			break;
		}
	}
	temp->moveAbsolute(vector);

	switch (autoGenerationIndex % 3) {
		case 0 : {
			vector.setVector(0.02, 0.04, 0);
			break;
		}
		case 1 : {
			vector.setVector(-0.01, 0.01, 0.04);
			break;
		}
		case 2 : {
			vector.setVector(0, -0.04, 0.02);
			break;
		}
	}
	temp->setVelocity(vector);

	addObject(temp);
	autoGenerationIndex++;
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

void Field::addForce(Force* force)
{
	this->force.add(force);
}
/*
void Field::finishForce(short idx)
{
	delete force[idx];
	force.remove(idx);
}
*/
Field* Field::field = NULL;
